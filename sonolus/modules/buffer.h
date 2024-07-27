#ifndef BUFFER_H
#define BUFFER_H

using namespace std;
class buffer {
    private:

    public:
    vector<char> v;
    buffer(){v.clear();}
    buffer(string res) {
        v.clear();
        for (int i = 0; i < res.size(); i++) v.push_back(res[i]);
    }
    buffer(char* ch, int len) {
        v.clear();
        for (int i = 0; i < len; i++) v.push_back(ch[i]);
    }
    
    void push(char x) {
        v.push_back(x);
    }
    string asString() {
        string res;
        for (int i = 0; i < v.size(); i++) res.push_back(v[i]);
        return res;
    }
    const char* asCString() {
        string res;
        for (int i = 0; i < v.size(); i++) res.push_back(v[i]);
        return res.c_str();
    }
    int size() {
        return v.size();
    }
    void clear() {
        v.clear();
    }
    buffer substr(int st, int len = (1ll << 31) - 1) {
        if (st < 0) st = size() + st;
        buffer res; for (int i = 0, j = st; i < len && j < v.size(); i++, j++) res.push(v[j]);
        return res;
    }
};

#endif