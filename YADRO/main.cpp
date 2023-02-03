#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter) {
    std::vector<std::string> result;
    const std::string& str = string;
    size_t find1;
    size_t find2 = -delimiter.size();
    do {
        find1 = find2 + delimiter.size();
        find2 = str.find(delimiter, find1);
        result.push_back(str.substr(find1, find2 - find1));
    } while (find2 != -1);
    return result;
}

std::string Find(std::vector<std::vector<std::string> >& table,
                 std::vector<std::string>& strings,
                 const std::string& str, const std::string& col) {
    size_t it_str = std::find(strings.begin(), strings.end(), str) - strings.begin();
    size_t it_col = std::find(table[0].begin(), table[0].end(), col) - table[0].begin();
    if (it_str == strings.size() or it_col == table[0].size()) {
        std::string error = "Item with number " + col + str + " " + "does not exist";
        throw std::runtime_error(error);
    }
    return table[it_str][it_col];
}

std::string Count(const std::string& expresion, std::vector<std::vector<std::string> >& table,
                  std::vector<std::string>& strings);

int calculate(const std::string& expresion, std::vector<std::vector<std::string> >& table,
        std::vector<std::string>& strings, std::string sign) {
    std::string expr = expresion;
    std::vector<std::string> args = Split(expr.erase(0, 1), sign);

    std::string name1;
    std::string number1;

    std::string name2;
    std::string number2;

    for (char index : args[0]) {
        if (!isdigit(index)) {
            name1.push_back(index);
        } else {
            number1.push_back(index);
        }
    }

    for (char index : args[1]) {
        if (!isdigit(index)) {
            name2.push_back(index);
        } else {
            number2.push_back(index);
        }
    }

    std::string first, second;

    if (!name1.empty()) {
        first = Find(table, strings, number1, name1);
        if (first[0] == '=') {
            first = Count(first, table, strings);
        }
    } else {
        first = number1;
    }

    if (!name2.empty()) {
        second = Find(table, strings, number2, name2);
        if (second[0] == '=') {
            second = Count(second, table, strings);
        }
    } else {
        second = number2;
    }

    switch (sign[0]) {
        case '+' :
            return std::stoi(first) + std::stoi(second);
        case '-' :
            return std::stoi(first) - std::stoi(second);
        case '*' :
            return std::stoi(first) * std::stoi(second);
        case '/' :
            if (std::stoi(second) != 0) {
                return std::stoi(first) / std::stoi(second);
            } else {
                throw std::runtime_error("Division by zero error");
            }
    }
    throw std::runtime_error("Something went wrong");
}

std::string Count(const std::string& expresion, std::vector<std::vector<std::string> >& table,
                  std::vector<std::string>& strings) {
    int plus = expresion.find('+');
    int minus = expresion.find('-');
    int multi = expresion.find('*');
    int divid = expresion.find('/');
    if (plus != -1) {
        return std::to_string(calculate(expresion, table, strings, "+"));
    } else if (minus != -1) {
        return std::to_string(calculate(expresion, table, strings, "-"));
    } else if (multi != -1) {
        return std::to_string(calculate(expresion, table, strings, "*"));
    } else if (divid != -1) {
        return std::to_string(calculate(expresion, table, strings, "/"));
    } else {
        throw std::runtime_error("Incorrect input");
    }

}

void print_table(std::vector<std::vector<std::string> > table) {
    for (size_t index = 0; index < table.size(); index++) {
        for (size_t jindex = 0; jindex < table[0].size(); jindex++) {
            std::cout << table[index][jindex] << ' ';
        }
        std::cout << '\n';
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        throw std::runtime_error("Bad input");
    }
    char* path = argv[1];
    std::ifstream myFile;
    myFile.open(path, std::ios::in);
    if (!myFile.is_open()) {
        throw std::runtime_error("Can't read data");
    }

    std::vector<std::vector<std::string> > table;
    std::string s;
    std::vector<std::string> strings;

    //create names of columns
    while (getline(myFile, s)) {
        std::vector<std::string> str = Split(s, ",");
        table.push_back(str);
        strings.push_back(str[0]);
    }

    myFile.close();

    for (size_t index = 0; index < table.size(); ++index) {
        for (size_t jindex = 0; jindex < table[0].size(); ++jindex) {
            if (table[index][jindex][0] == '=') {
                table[index][jindex] = Count(table[index][jindex], table, strings);
            }
        }
    }

    print_table(table);

    return 0;
}