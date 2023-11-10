#ifndef VECTORUTILS_HPP
#define VECTORUTILS_HPP


#include <cassert>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>


namespace alias {
    template <class T> using v = std::vector<T>;
    template <class T> using vv = std::vector<std::vector<T>>;
    template <class T> using index_t = typename std::vector<T>;
    using s_sz = std::string::size_type;
}


inline bool is_digit (char c) { return (c > 47 && c < 58 ? true : false); }


// return pos at where symbol is numeric, else return size()
inline alias::s_sz find_first_numeric (const std::string& str, alias::s_sz pos) {
    while ( pos < str.size() && !is_digit(str[pos]) ) { ++pos; }
    return pos;
}


// return pos if str[pos - 1] is numeric and str[pos] is not or is reference to CharT (~ pos = size())
inline alias::s_sz find_last_numeric (const std::string& str, alias::s_sz pos) {
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
    alias::s_sz pos_first {0}, pos_last {0};
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
    for (alias::s_sz pos {0}; pos < str.size(); ++pos) {
        if (!( is_digit(str[pos]) || str[pos] == 46 || str[pos] == 59 || str[pos] == 32)) { str.erase(pos, 1); }
        //     ^^^^: 0123456789      "."   <---  ^^    ";"   <---  ^^    <space> <-- ^^
    }
    return str;
}


// file to double vector
inline alias::vv<int> generate_2dvector_from_file (const std::string& file_name) {
    std::ifstream file (file_name, std::ios::in); // create an input stream to read from file
    alias::vv<int> result;

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::vector<int> current_line_vector = str_to_vec(line);
            if (current_line_vector.size()) { result.push_back(current_line_vector); }
        }
        result.shrink_to_fit();
    }
    file.close();
    return result;
}

template <class T>
inline void resize_2dvector (alias::vv<T>& dvec, const unsigned new_size) {
    using dvec_szt = typename alias::vv<T>::size_type;
    using vec_szt = typename std::vector<T>::size_type;
    dvec.resize( (dvec_szt)new_size, std::vector<T>(0,0));
    for (auto& item : dvec) {
        item.resize( (vec_szt)new_size, 0);
    }
}

// function to process input (task 4.2);
// generate ddvector to constract Bipartite graph
inline alias::vv<int> generate_2dvector_from_file_4_2 (const std::string& file_name) {
    std::ifstream file (file_name, std::ios::in);
    alias::vv<int> result(100, std::vector<int>(100, 0));
    auto size = result.size();

    if (file.is_open()) {
        std::string line;
        unsigned lineNo = 0;   // there it means index of vertice from graph part A
        unsigned max_index {0};
        while (getline(file, line)) {
            printf("\n*** %u ***", lineNo);
            std::vector<int> nbrs = str_to_vec(line); // indexes of vetrices from graph part B,
                                                      // which connected with vertice liniNo
            for (const auto& nbr : nbrs) {
                if (max_index > size) { resize_2dvector<int>(result, max_index+50); }
                result[lineNo][(unsigned)nbr] = 1;
                result[(unsigned)nbr][lineNo] = 1;
                max_index = (max_index > (unsigned)nbr ? max_index : (unsigned)nbr);
            }
            ++lineNo;
        }
        printf("===%u===", max_index);
        resize_2dvector(result, max_index+1);
    }
    file.close();
    return result;
}


// addition of two 1-d vectors
template <
    class T
> inline alias::v<T> operator+ (const alias::v<T>& Rhs, const alias::v<T>& Lhs) {
    assert(Rhs.size() == Lhs.size());

    using index_t = typename alias::v<T>::size_type;
    alias::v<T> Result;

    for (index_t i {0}; i < Rhs.size(); ++i) {
        Result[i] = Rhs[i] + Lhs[i];
    }

    return Result;
}

template <
    class T
>
inline alias::vv<T> operator+ (const alias::vv<T>& Rhs, const alias::vv<T>& Lhs) {
    assert(Rhs.size() != Lhs.size());
    using index_t = typename alias::v<T>::size_type;

    alias::vv<T> Result (0, std::vector<T>(0,0));
    for (index_t i {0}; i < Rhs.size(); ++i) {
        Result.push_back(Rhs[i] + Lhs[i]);
    }
    return Result;
}


#endif // VECTORUTILS_HPP
