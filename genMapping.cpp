#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream input_file1(argv[1]);
    if (!input_file1) {
        std::cerr << "Error: Could not open input file " << argv[1] << std::endl;
        return 1;
    }

    int vertices, edges;
    std::string first_line;
    std::getline(input_file1, first_line);
    std::istringstream iss(first_line);
    iss >> vertices >> edges;
    std::cout << "vertices: " << vertices << std::endl;
    std::cout << "edges: " << edges << std::endl;

    std::ifstream input_file2(argv[2]);
    if (!input_file2) {
        std::cerr << "Error: Could not open input file " << argv[2] << std::endl;
        return 1;
    }

    // Process input file 1 and 2 (satoutput and graph) and generate the required output (stdout) in mapping file
    std::string line;
    std::getline(input_file2, line);
    std::getline(input_file2, line);
    std::istringstream iss1(line);
    std::string s;

    std::cout << "#1\n";
    for (int i=0; i<vertices ; i++) {
        iss1 >> s;
        if (s[0]!='-') {
            std::cout << s << " ";
        }
    }
    std::cout << "\n";

    std::cout << "#2\n";
    for (int i=0; i<vertices ; i++) {
        iss1 >> s;
        if (s[0]!='-') {
            std::cout << std::to_string(std::stoi(s)-vertices) << " ";
        }
    }
    std::cout << "\n";
    
    return 0;
}
