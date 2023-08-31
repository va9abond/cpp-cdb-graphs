#ifndef VECTORUTILS_HPP
#define VECTORUTILS_HPP


#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>


using s_sz = std::string::size_type;


inline bool is_digit (char c) { return (c > 47 && c < 58 ? true : false); }


// return pos at where symbol is numeric, else return size()
inline s_sz find_first_numeric (const std::string& str, s_sz pos) {
    while ( pos < str.size() && !is_digit(str[pos]) ) { ++pos; }
    return pos;
}


// return pos if str[pos - 1] is numeric and str[pos] is not or is reference to CharT (~ pos = size())
inline s_sz find_last_numeric (const std::string& str, s_sz pos) {
    // static_assert(is_digit(str[pos - 1]));
    while (pos < str.size() && ( is_digit(str[pos]) || str[pos] == 46 )) { ++pos; }
    return pos;
}


// [WARNING]: Val_t should have conversation to std::string with
//            std::{ stoi, stol, stoll, stof, stod, stold, stoul, stoull } or
//            with user-defined function
// [TODO]: make a template; try std::vector<decltype(auto)>
inline std::vector<int> str_to_vec (const std::string &str) {
    std::vector<int> result;
    s_sz pos_first {0}, pos_last {0};
    for (
      pos_first = find_first_numeric(str, pos_first);
      pos_first < str.size();
      pos_first = pos_last + 1, pos_first = find_first_numeric(str, pos_first)
   ) {
        pos_last  = find_last_numeric(str, pos_first + 1);
        result.push_back( std::stoi(str.substr(pos_first, pos_last - pos_first)) );
    }
    return result;
}


// "{ a, b, c, d, e };" --> " a b c d e;"
// [TODO]: this version of function delete all spaces!!
inline std::string normalize_string (std::string str) {
    for (s_sz pos {0}; pos < str.size(); ++pos) {
        if (!( is_digit(str[pos]) || str[pos] == 46 || str[pos] == 59 || str[pos] == 32)) { str.erase(pos, 1); }
        //     ^^^^: 0123456789      "."   <---  ^^    ";"   <---  ^^    <space> <-- ^^
    }
    return str;
}


// file to double vector
inline std::vector<std::vector<int>> generate_2dvector_from_file (const std::string& file_name) {
    std::ifstream file (file_name, std::ios::in); // create an input stream to read from file
    std::vector<std::vector<int>> result;

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::vector<int> current_line_vector = str_to_vec(line);
            if (current_line_vector.size()) { result.push_back(current_line_vector); }
        }
    }
    file.close();
    return result;
}

#endif // VECTORUTILS_HPP
