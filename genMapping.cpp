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
    input_file1 >> vertices >> edges;
    // std::cout << "vertices: " << vertices << std::endl;
    // std::cout << "edges: " << edges << std::endl;

    std::ifstream input_file2(argv[2]);
    if (!input_file2) {
        std::cerr << "Error: Could not open input file " << argv[2] << std::endl;
        return 1;
    }

    // Process input file 1 and 2 (satoutput and graph) and generate the required output (stdout) in mapping file
    std::string line;
    input_file2 >> line;
    
    if (line=="UNSAT") std::cout << 0;
    else {
        int c1 = 0;
        int c2 = 0;
        std::string s;
        // std::cout << "#1\n";
        for (int i=0; i<vertices ; i++) {
            input_file2 >> s;
            if (s[0]!='-') {
                c1++;
                // std::cout << s << " ";
            }
        }
        // std::cout << "\n";
        // std::cout << "#2\n";
        for (int i=0; i<vertices ; i++) {
            input_file2 >> s;
            if (s[0]!='-') {
                c2++;
                // std::cout << std::to_string(std::stoi(s)-vertices) << " ";
            }
        }

        std::ifstream input_file3(argv[2]);
        input_file3 >> s;
        std::cout << "#1\n";
        int c3 = 0;
        int c4 = 0;
        for (int i=0; i<vertices ; i++) {
            input_file3 >> s;
            if (s[0]!='-') {
                c3++;
                if (c3==c1) std::cout << s;
                else std::cout << s << " ";
            }
        }
        std::cout << "\n";
        std::cout << "#2\n";
        for (int i=0; i<vertices ; i++) {
            input_file3 >> s;
            if (s[0]!='-') {
                c4++;
                if (c4==c2) std::cout << std::to_string(std::stoi(s)-vertices);
                else std::cout << std::to_string(std::stoi(s)-vertices) << " ";
            }
        }


    }
    
    return 0;
}
