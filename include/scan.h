#ifndef scanh
#define scanh

#include <bits/stdc++.h>
using namespace std;

//src==source file, hd==header file, dir==directory, grp=graph
class Scanner{
public:
    void scandir(const filesystem::path& testdir); //scans dir for all srcs and hds 
    void buildgrp();//builds grp of inter-file dependencies
    void grpout() const;//print grp
    void printsum(chrono::microseconds duratn, int compct, int skipct) const;//prints summary of build duration
    void clearspace(const filesystem::path& testdir);//clears space of generated files
    void aireview(const filesystem::path& testdir);//

    const vector<filesystem::path>& gtsrcfile()const{return srcfile;}// gets srcs
    const vector<filesystem::path>& gthdfile()const{return hdfile;}// gets hds
    const unordered_map<string,vector<string>>& gtgrp( )const{return grpfile;}//gets grps
    bool findcompord(); //returns false if a longer cyclic dependency is found
    const vector<string>& gtcompord() const { return compord; }
    bool rebuildchk(const filesystem::path& src, const filesystem::path& obj);//check if src needs to be recompiled
    bool exbuild(const filesystem::path& testdir); //executing the build
    bool validgrp(const filesystem::path& root) const;//check graph integrity

private:
    vector<filesystem::path>srcfile; //stores srcs i.e. .cpp files
    vector<filesystem::path>hdfile;// stores hds i.e. .h files
    unordered_map<string,vector<string>>grpfile;//stores the file-dependency grp
    vector<string> compord;
    
    void processdeps(const filesystem::path& file);//dependency processer
    vector<string>extractinclude(const filesystem::path& fdir);//extracts the include for header dependencies
    bool toposort(const string& node, unordered_map<string, int>& vistat);//toposort
};

#endif