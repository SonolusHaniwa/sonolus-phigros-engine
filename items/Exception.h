using namespace std;

class function_decrypted: public exception {
    public:

    string error = "A decrypted function was called.";
    function_decrypted(string error): error(error){}

    const char* what() const throw() {
        return error.c_str();
    }
};