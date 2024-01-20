#include<zip.h>
using namespace std;

string getFileFromZip(string zipPath, string filePath) {
    zip* z = zip_open(zipPath.c_str(), 0, NULL);
    if (z == NULL) {
        throw runtime_error("Cannot open zip file.");
        exit(3);
    }
    zip_stat_t st;
    zip_stat_init(&st);
    zip_stat(z, filePath.c_str(), 0, &st);
    char* contents = new char[st.size];
    zip_file_t* f = zip_fopen(z, filePath.c_str(), 0);
    if (f == NULL) {
        throw runtime_error("Cannot open file in zip.");
        exit(3);
    }
    zip_fread(f, contents, st.size);
    zip_fclose(f);
    zip_close(z);
    string ret = string(contents, st.size);
    delete[] contents;
    return ret;
}

void writeFileIntoZip(string zipPath, string filePath, string fileContent) {
    zip* z = zip_open(zipPath.c_str(), ZIP_CREATE, NULL);
    if (z == NULL) {
        throw runtime_error("Cannot open zip file.");
        exit(3);
    }
    zip_source_t* s = zip_source_buffer(z, fileContent.c_str(), fileContent.size(), 0);
    zip_add(z, filePath.c_str(), s);
    zip_close(z);
}