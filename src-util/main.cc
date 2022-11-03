#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

std::vector<uint8_t> out = {0xf0};

void replace(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

int main(int argc, char **argv){
    std::ifstream memorycfg("memory.cfg");
    for(std::string line; std::getline(memorycfg, line);){
        std::vector<std::string> splitLine = split(line, "=");
        replace(splitLine[1], "\\n", "\n");
        std::vector<char> name(splitLine[0].begin(), splitLine[0].end());
        std::vector<char> value(splitLine[1].begin(), splitLine[1].end());
        for(char ch : name)
            out.push_back((uint8_t) ch);
        out.push_back(0xfe);
        for(char ch : value)
            out.push_back((uint8_t) ch);
        out.push_back(0xfe);
    }
    out.push_back(0xf1);
    std::ifstream file(argv[1]);
    std::vector<std::string> lines;
    for(std::string line; std::getline(file, line);){
        lines.push_back(line);
    }
    for(std::string line : lines){
        std::vector<std::string> splitLine = split(line, " ");
        if(splitLine[0] == "mov"){
            if(splitLine[1].c_str()[1] == 'x'){
                if(splitLine[2].c_str()[1] == 'x'){

                } else {
                    if(splitLine[1].c_str()[0] == '1'){
                        out.push_back(0x10);
                        out.push_back((uint8_t) (splitLine[1].c_str()[2]-'0') * 10 + (splitLine[1].c_str()[3]-'0'));
                        for(size_t pos = 0; pos < strlen(splitLine[2].c_str()); pos++)
                            out.push_back((uint8_t) splitLine[2].c_str()[pos]);
                        out.push_back(0xfe);
                    }
                }
            } else {
                if(splitLine[2].c_str()[1] == 'x'){

                } else {

                }
            }
        } else if(splitLine[0] == "int"){
            out.push_back(0x20);
            out.push_back((uint8_t) (splitLine[1].c_str()[0]-'0') * 10 + (splitLine[1].c_str()[1]-'0'));
        }
    }
    std::ofstream outfile(argv[2]);
    for(uint8_t ch : out)
        outfile << ch;
    
}