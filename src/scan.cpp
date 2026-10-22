#include "scan.h"
#include <bits/stdc++.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") //Links Winsock library automatically on MSVC
    #define CLOSE_SOCKET closesocket
    typedef int socklen_t;
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET close
#endif
bool initNetwork() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

void cleanupNetwork() {
#ifdef _WIN32
    WSACleanup();
#endif
}
using namespace std;

void Scanner::scandir(const filesystem::path& dir) {
    if (!filesystem::exists(dir) || !filesystem::is_directory(dir)) {//raise error when path DNE or not a dir
        cerr<< "\033[31m Error \033[0m :Target path not a valid directory.\n";
        return;
    }
    srcfile.clear();//clear any previously stored srcs or hds
    hdfile.clear();

    for (const auto& inp : filesystem::recursive_directory_iterator(dir)) {//dfs through dir files
        if (filesystem::is_regular_file(inp.path())) {
            string ext = inp.path().extension().string();
            
            if (ext == ".cpp" || ext == ".cc") {
                srcfile.push_back(inp.path());
            } else if (ext == ".h" || ext == ".hpp") {
                hdfile.push_back(inp.path());
            }
        }
    }
}
vector<string>Scanner::extractinclude(const filesystem::path& dir) {
    vector<string> depend;
    ifstream file(dir);
    
    if (!file.is_open()) return depend;
    string inp;// store input 
    while (getline(file, inp)) {
        inp.erase(inp.begin(), find_if(inp.begin(), inp.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));//remove whitespaces in string
        if (inp.rfind("#include \"", 0) == 0) {//look for the start of hd file
            size_t st = inp.find("\"") + 1;//start of filename
            size_t en = inp.find("\"", st);//end of filename
            
            if (en != string::npos) {
                string hdname = inp.substr(st, en - st);
                depend.push_back(hdname);//push filename
            }
        }
    }
    return depend;
}
void Scanner::processdeps(const filesystem::path& file){
    string fn= file.filename().string();
    vector<string> indep= extractinclude(file);// initial dependencies
    vector<string> chkdep;//valid dependencies

    for (const std::string& dep:indep) {
        if (fn== dep){//self-loop detection
            cout<< "\033[31m Error \033[0m: Self-loop detected in " <<fn << ",file cannot include itself. Dropping this edge.\n";
            continue;
        }
        bool local= false;//check if header file in dir scope
        filesystem::path hdpath;
        filesystem::path depfn= filesystem::path(dep).filename();//extract just the filename from the path

        for (const auto& hf: hdfile) {// check in hd files
            if(hf.filename()==depfn){//ensure subpath matches trailing strings
                string hs=hf.string();
                if (hs.length()>=dep.length() && hs.substr(hs.length()-dep.length())==dep){//if entire path included
                    local= true;hdpath= hf;break;
                }
                if (dep==depfn.string()) {//if just the filename included
                    local= true;hdpath=hf;break;
                }
            }
        }
        for (const auto& sf:srcfile) {//check src files in case
            if(sf.filename().string()== dep) { //depth scan through src files,breaking at .cpp
                string ss=sf.string();
                if ((ss.length()>=dep.length() && ss.substr(ss.length()-dep.length())==dep) || dep==sf.filename().string()) {
                    local= true;
                    hdpath= sf;
                    if (sf.extension() == ".cpp") {
                        cout<< "\033[33m Warning:\033[0m "<<fn<<" includes a source file directly: '" << dep << "'.\n";
                    }
                    break;
                }
            }
        }

        if(!local) {//in case of external reference
            cout<< "\033[33m NOTE \033[0m: "<<fn<<" references " << dep<< " which does not exist locally in this directory.\n";
        }

        chkdep.push_back(hdpath.filename().string());//push only the base filename
    }
    grpfile[fn]=chkdep;
}
void Scanner::buildgrp(){
    grpfile.clear();

    if(srcfile.empty()&& hdfile.empty()) {
    cout<<"\033[33m Warning \033[0m :No valid C++ source or header files detected in target directory.\n";
    return;}

    for (const auto& file:srcfile){//map src 
        string fn=file.filename().string();
        grpfile[fn] = extractinclude(file);
    }
    for (const auto& file : hdfile) {//map hd
        string fn= file.filename().string();
        grpfile[fn] = extractinclude(file);
    }
    
}
void Scanner::grpout() const {
    cout<< "~~~Dependency Graph(Adjacency List)~~~\n";
    for (const auto& [file, deps] : grpfile) {
        cout<<file<<" -> ";
        if (deps.empty()) {
            cout<<"[No internal dependencies]";
        } else {
            for (size_t i = 0; i < deps.size(); ++i) {
                cout<<deps[i]<<(i==deps.size()-1?"":", ");
            }
        }
        cout << "\n";
    }
    cout << "---------------------------------------\n";
}
bool Scanner::toposort(const string& node, unordered_map<string, int>& vistat) {
    if(vistat[node] == 1) return false;//state 1 implies hit a node on stack i.e. cycle detection
    if(vistat[node] == 2) return true;// state 2 implies no clash
    vistat[node] = 1;//mark as currently visiting

    if(grpfile.find(node) != grpfile.end()) {//traverses all file the node depends on
        for(const string& nbr:grpfile.at(node)) {
            // Only traverse neighbors that exist inside our project files
            if(grpfile.find(nbr) != grpfile.end() || vistat.find(nbr) != vistat.end()) {//only traverse neighbours inside project file
                if(!toposort(nbr, vistat)) return false;
            }
        }
    }
    vistat[node] = 2;//mark as fully processed
    compord.push_back(node);
    return true;
}

bool Scanner::findcompord() {
    compord.clear();
    unordered_map<string, int>vistat;

    for(const auto&[file,deps] :grpfile) {//known nodes initialised 0(not visit)
        vistat[file]= 0;
    }
    for (const auto&[file,deps] :grpfile){//run dfs from the unvisit-marked
        if (vistat[file] == 0) {
            if (!toposort(file, vistat)) {
                std::cout << "\033[31m Error:\033[0m Cyclic dependency loop found! Compilation halted.\n";
                compord.clear();
                return false;
            }
        }
    }
    return true;
}
size_t computefhash(const filesystem::path& path) {//compute hash for file
    ifstream file(path,ios::binary);
    if (!file.is_open()) return 0;

    const size_t bufferSize = 8192;
    vector<char> buffer(bufferSize);
    hash<string> hasher;
    size_t hashc = 0;

    while(file){
        file.read(buffer.data(), bufferSize);
        streamsize bytread= file.gcount();
        if(bytread>0) {
            string block(buffer.data(), bytread);
            hashc^= hasher(block) + 0x9e3779b9 + (hashc << 6) + (hashc >> 2);
        }
    }
    return hashc;
}
bool Scanner::rebuildchk(const filesystem::path& src,const filesystem::path& obj){
    if(!filesystem::exists(obj)) return true;//if compiled binary file DNE
    size_t curr=computefhash(src);
    filesystem::path cachePath= src.parent_path() / ".gaia_cache";
    unordered_map<string, size_t> cache;
    
    if (filesystem::exists(cachePath)) {
        ifstream cacheIn(cachePath);
        string filename;
        size_t old;
        while (cacheIn >>filename >>old) {
            cache[filename]= old;
        }
    }

    string fn=src.filename().string();
    bool changed= true;

    if (cache.find(fn)!=cache.end() && cache[fn]==curr) {
        changed = false; //unchanged, skip build
    }

    cache[fn]= curr;
    ofstream cacheOut(cachePath);
    for(const auto& [file, h] : cache) {
        cacheOut<< file <<" " << h<< "\n";
    }

    if(changed) {// rebuild if changes
        cout <<"[Gaia-Create] Hash mismatch for "<< fn << ". Rebuilding...\n";
        return true;
    }

    return false;// file unchanged
}
string gtincflags(const filesystem::path& root) {
    string flags="";
    for(const auto& entry:filesystem::recursive_directory_iterator(root)) {
        if (entry.is_directory()){
            for(const auto& file:filesystem::directory_iterator(entry)){//check dir for .h/.hpp files
                if(file.path().extension()==".h" || file.path().extension()==".hpp") {
                    flags+= "-I\"" + entry.path().string() + "\" ";
                    break; //ensure only add once
                }
            }
        }
    }
    return flags;
}
mutex cout_mutex;

void Scanner::compile_instr(const filesystem::path& root){
    filesystem::path jsonP= root/ "compile_commands.json";
    ofstream jsonFile(jsonP);

    if(!jsonFile.is_open())return;

    jsonFile<<"[\n";
    string incflags = gtincflags(root);
    bool first = true;

    for(const auto& sf : srcfile) {//for each source file, convert to appropriate compilation command
        if (!first) jsonFile << ",\n";
        first = false;

        filesystem::path obj = sf;
        obj.replace_extension(".o");

        string cmd= "g++ -std=c++17 " + incflags + " -c \"" + sf.string() + "\" -o \"" + obj.string() + "\"";//make compile command
        jsonFile << "  {\n"<< "    \"directory\": \"" << root.string() << "\",\n"<< "    \"file\": \"" << sf.string() << "\",\n"<< "    \"command\": \"" << cmd << "\"\n"<< "  }";
    }
    jsonFile << "\n]\n";
    jsonFile.close();
}

bool Scanner::exbuild(const filesystem::path& testdir) {
    auto strt = chrono::high_resolution_clock::now();
    int processct = 0, compct = 0, skipct = 0, totalsrc = 0;

    for (const std::string& fn : compord) {
        if (filesystem::path(fn).extension() == ".cpp") totalsrc++;
    }

    cout << "\033[34m Executing Parallel Smart-Build ...\033[0m\n\n";
    
    bool changes = false;
    string incflags = gtincflags(testdir);
    std::vector<std::future<bool>> futures;

    for (const std::string& fn : compord) {
        if (filesystem::path(fn).extension() != ".cpp") continue;
        processct++;

        filesystem::path src;
        for (const auto& sf : srcfile) {
            if (sf.filename().string() == fn) { src = sf; break; }
        }

        filesystem::path obj = src;
        obj.replace_extension(".o");
        string progress = "[" + std::to_string(processct) + "/" + to_string(totalsrc) + "]";

        if (rebuildchk(src, obj)) {
            // Launch compilation in background
            futures.push_back(std::async(std::launch::async, [=]() {
                string cmd = "g++ -std=c++17 " + incflags + " -c \"" + src.string() + "\" -o \"" + obj.string() + "\"";
                
                std::lock_guard<std::mutex> lock(cout_mutex);
                cout << "\033[35m" << progress << "\033[0m[Compiling] " << fn << "\n";
                
                return (system(cmd.c_str()) == 0);
            }));
            compct++;
            changes = true;
        } else {
            cout << "\033[32m" << progress << "[Updated] " << fn << "\n";
            skipct++;
        }
    }

    // Wait for all compilations to complete and check for errors
    for (auto& f : futures) {
        if (!f.get()) {
            cerr << "\033[31m Error: A background compilation task failed.\033[0m\n";
            return false;
        }
    }

    // Linking Phase (sequential)
    filesystem::path bintest = testdir / "gaia_output_bin";
    if (changes || !filesystem::exists(bintest)) {
        string linkcmd = "g++ -std=c++17 ";
        for (const auto& sf : srcfile) {
            filesystem::path otest = sf;
            otest.replace_extension(".o");
            linkcmd += "\"" + otest.string() + "\" ";
        }
        linkcmd += "-o \"" + bintest.string() + "\"";
        
        cout << "\n[Linking Executable] " << bintest.filename().string() << "\n";
        if (system(linkcmd.c_str()) != 0) {
            cerr << "\033[31m Error: Object linking failed.\033[0m\n";
            return false;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duratn = chrono::duration_cast<chrono::microseconds>(end - strt);
    cout << "\n\033[32m Build sequence completed successfully!\033[0m\n";
    printsum(duratn, compct, skipct);
    return true;
}
void Scanner::printsum(chrono::microseconds duratn, int compct, int skipct) const{//printing smart build metrice
    double ms =duratn.count()/1000.0;
    cout<<"\n\033[95m Gaia-Create's smart build performance-\033[0m\n"
              << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
              << " 1. Total Build Time Processing- " << ms << " ms\n"
              << " 2. Targets Recompiiled- " <<compct<< "\n"
              << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
    if(compct== 0 && skipct>0) {
        cout<< " \033[32m Zero overhead! Incremental compilation saved 100% of standard build times.\033[0m\n\n";
    }
}
void Scanner::clearspace(const filesystem::path& testdir) {
    cout<<"\033[35m Scanning workspace for build artifacts...\033[0m\n";
    int delct=0;

    if (!filesystem::exists(testdir) || !filesystem::is_directory(testdir)) {
        cerr << "Error: Invalid directory path targeted for cleaning.\n";
        return;
    }

    for (const auto& entry:filesystem::recursive_directory_iterator(testdir)) {
        const auto& path =entry.path();
        if (path.extension() == ".o" || path.filename() == "gaia_output_bin"|| path.filename() == "compile_instr.json" || path.filename() == ".gaia_cache") {
            try {
                filesystem::remove(path);
                cout << "[Removed] " << path.filename().string() << "\n";
                delct++;
            } catch (const filesystem::filesystem_error& e) {
                cerr << "Error: Could not remove " << path << endl;
            }
        }
    }
    cout << "\033[32m Workspace pristine! " << delct << " artifact(s) removed.\033[0m\n\n";
}
bool Scanner::validgrp(const filesystem::path& root) const{
    bool pass=true;
    for(const auto& [file, deps]:grpfile) {
        for(const string& dep:deps){
            if(dep.find('<') != string::npos || dep.find('>') != string::npos) continue;//ignore system libraries like <iostream>
            if (!filesystem::exists(root / dep)){//check path existence relative to project root
                cerr<<"\033[31m Error:\033[0m "<<file<<" references missing file '\033[4m"<<dep<<"\033[0m'.\n"<< "  Fix: Please verify that "<<dep<<" exists relative to "<<root<<"\n";
                pass = false;
            }
        }
    }
    return pass;
}
string formatreview(const string& text){//response formatting helpfer for aireview
    string txt=text;

    size_t pos = 0;//make bold (**text**) to bold white on terminal
    bool bold=true;
    while((pos=txt.find("**",pos))!=string::npos){
        if(bold) txt.replace(pos, 2, "\033[1;37m");
        else txt.replace(pos, 2, "\033[0m");//reset
        bold=!bold;
    }

    pos=0;//code ticks('code') to bright yellow
    bool code=true;
    while((pos=txt.find("`", pos))!=string::npos) {
        if (code) txt.replace(pos, 1, "\033[93m"); 
        else txt.replace(pos, 1, "\033[0m"); //reset
        code=!code;
    }

    while((pos=txt.find("```cpp"))!=string::npos)txt.erase(pos, 6);//cleanup display
    while((pos=txt.find("```"))!=string::npos)txt.erase(pos, 3);

    pos = 0;//bullet points to magenta
    while((pos = txt.find("\n* ", pos))!=string::npos) {
        txt.replace(pos, 3, "\n \033[35m•\033[0m "); 
    }

    return txt;
}
void Scanner::aireview(const filesystem::path& testdir) { // ai review using ollama api bridge for llms
    (void)testdir;//just for bypassing warnings
    vector<filesystem::path> all;
    for (const auto& f:srcfile) {
        if (find(all.begin(), all.end(),f) == all.end())all.push_back(f);
    }
    for (const auto& f:hdfile) {
        if (std::find(all.begin(), all.end(),f) == all.end()) all.push_back(f);
    }

    if(all.empty()) {
        cout << "No codes discovered to review.\n";
        return;
    }
    for(const auto& fp:all) {
        cout<< "\n--------------------------------------------------------\n"<< "Analyzing \033[36m" << fp.filename().string() << "\033[0m ...\n";
        ifstream file(fp); // read file contents into memory string
        if(!file.is_open()) continue;
        stringstream buffer;
        buffer << file.rdbuf();
        string content =buffer.str();
        string escontent ="";
        for(char c : content){ // retrieve the escaped char like newline
            if(c =='"')escontent += "\\\"";
            else if(c =='\\')escontent += "\\\\";
            else if(c =='\n')escontent += "\\n";
            else if(c =='\r')escontent += "\\r";
            else if(c =='\t')escontent += "\\t";
            else escontent += c;
        }

        //prompt for local model
        string prompt ="Analyze the provided C++ source code for: 1. Pattern Recognition: Identify boilerplate or over-engineered logic that can be simplified.2. Style Consistency: Check for structural symmetry, naming conventions, and comment quality.3. Integrity Assessment: Analyze structural patterns for potential AI-generated code characteristics.4. Optimization Hints: Suggest specific ways to refactor complex dependency logic to reduce technical debt.Format the response in clear, concise bullet points. Code:" + escontent;

        ofstream payload(".payload.json");//temporary file having payload to prevent inline parsing errors
        if (!payload.is_open()) {
            cerr << "Failed to create temporary payload configuration file.\n";
            continue;
        }
        payload << "{\n  \"model\": \"llama3.2:1b\",\n  \"prompt\": \"" << prompt << "\",\n  \"stream\": false\n }";
        payload.close();

        //full system curl command targetting the 'phi3' engine,'stream: false' so that Ollama returns entire response at once 
        string cmd = "curl -s -X POST http://localhost:11434/api/generate -d @.payload.json > .response.raw";
        int status = system(cmd.c_str()); // dispatch code block evaluation command to OS shell

        ifstream chk(".response.raw");//check if curl wrote an output
        bool has = chk.peek() != ifstream::traits_type::eof();
        chk.close();

        if (status != 0 || !has) {
            cerr << "\nFailed to communicate with Local Ollama Node! Ensure Ollama is running on your machine.\n";
            filesystem::remove(".payload.json");
            filesystem::remove(".response.raw");
            return;
        }

        string format= "python3 -c \"import json; data=json.load(open('.response.raw')); print(data.get('response', ''))\" > .response.txt";//output formatted in separate file parsing json via python
        int fstatus=system(format.c_str());
        if(fstatus!=0){
            cerr<<"Warning: Python formatting step exited with an error code.\n";
        }

        ifstream result(".response.txt");//read cleaned response into text buffer 
        stringstream buff;
        buff<<result.rdbuf();
        result.close();
        string airev=buff.str();

        airev=formatreview(airev);

        cout<<airev<< "\n";

        filesystem::remove(".payload.json");//remove temporary storage files
        filesystem::remove(".response.raw");
        filesystem::remove(".response.txt");
    }
    cout << "\n--------------------------------------------------------\n"<< "\033[32m AI Code Audit Complete!\033[0m All discovered targets analyzed successfully.\n";
}