#include <bits/stdc++.h>
#include "scan.h"

void chkslct(const char* pn) {//returns guide
    cout<< "Input->"<<pn<<"[mode] ./[dir]"<< "Modes:\n"<< "1. -l, --list:Print out the discovered directories and files structure.\n"<< "2. -g, --graph :Analyze file contents and print the dependency adjacency list.\n"<< "3. -b, --build :Perform a smart incremental build of the target project.\n"<<"4. -c, --clean :Purge all intermediate .o files and binary outputs.\n"<< "5. -r, --review: Run a deep LLM review of all source files' code quality.\n\n"<<"If no directory is provided, the current working directory will be scanned.\n";
}

int main(int argc, char* argv[]) {
    if(argc< 2) {//return guide if insufficient input
        chkslct(argv[0]);
        return 0;
    }
    string mode= argv[1];
    filesystem::path dir= (argc > 2)? argv[2]:filesystem::current_path();
    if(!filesystem::exists(dir) || !filesystem::is_directory(dir)) {//check dir existence
        cerr<<"\033[31m Error \033[0m:Target path " << dir.string() << " is not a valid directory.\n";
        return 0;
    }

    Scanner scanr;
    if(mode== "-g"||mode== "--graph") {//grp relation mode
        cout << "Target directory: " << filesystem::absolute(dir) <<"\n";
        cout << "Analyzing trace patterns for dependency graph ..\n\n";
        
        scanr.scandir(dir);
        scanr.buildgrp();
        scanr.grpout();
        cout<<"Calculating safe linear compilation order...\n";
        if (scanr.findcompord()) {
            cout<<"\033[32m✔ Success!\033[0m Build Sequence:\n  ";
            const auto& ord=scanr.gtcompord();
            for (size_t i= 0;i <ord.size();i++) {
                cout<<ord[i]<<(i ==ord.size()-1 ?"" :" ➔  ");
            }
            cout<< "\n";
        }
        
    }
    else if(mode == "-l"||mode == "--list"){//dir list mode
    cout << "Scanning contents of target directory : " << filesystem::absolute(dir) <<"\n\n";
    scanr.scandir(dir);
    cout << "~~~Found Source Files(.cpp)~~~\n";
    for (const auto& file : scanr.gtsrcfile()) {
        cout << "- " << filesystem::relative(file,dir).string() << "\n";
    }
    cout << "~~~Found Header Files(.h)~~~\n";
    for (const auto& file : scanr.gthdfile()) {
        std::cout << "- " << filesystem::relative(file,dir).string() << "\n";
    }
    }
    else if (mode == "-b" || mode == "--build") {
        scanr.scandir(dir);
        scanr.buildgrp();
        
        if(!scanr.findcompord()) {//compute toposort
            return 1; //cycle detected
        }
        if(!scanr.validgrp()){//check for missing local files before compilation/build
            cerr << "Build aborted:One or more local files are missing.\n";
            return 1;
        }
        if(!scanr.exbuild(dir)){//initiate automate build
            cerr<< "Build terminated due to execution errors.\n";
            return 1;
        }
    }
    else if(mode=="-c" || mode=="--clean"){scanr.clearspace(dir);}
    else if (mode=="-r" || mode=="--review") {
        scanr.scandir(dir);
        cout << "\033[35m Starting LLM core for code review ...\033[0m\n";
        scanr.aireview(dir);
    }
    else {
        cerr << "\033[31m Error \033[0m:Unknown execution flag ("<<mode<<")\n\n";
        chkslct(argv[0]);
        return 0;
    }
}