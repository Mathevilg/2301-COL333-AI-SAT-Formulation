#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: Could not open input file " << argv[1] << std::endl;
        return 1;
    }

    // Process input file (test.graphs) and generate the required output

    // number of vertices and edges of graph fetched from the first line of input graph file
    int vertices, edges;
    std::string first_line;
    std::getline(input_file, first_line);
    std::istringstream iss(first_line);
    iss >> vertices >> edges;
    std::cout << "vertices: " << vertices << std::endl;
    std::cout << "edges: " << edges << std::endl;


    // Construct an adjacency matrix by reading the graph
    std::vector<std::vector<bool> > adjacencyMatrix (vertices, std::vector<bool>(vertices, true));
    std::string line;
    int count = 0;
    while (std::getline(input_file, line)) {
            std::istringstream iss(line);
            int row, col;
            iss >> row >> col;
            adjacencyMatrix[row-1][col-1] = 0;
            adjacencyMatrix[col-1][row-1] = 0;
            count++;
            std::cout << line << std::endl;
    }
     
    // std::cout << "Hello\n";


    // iterate over all values of k from 1 to n
    int k = 1;
    int constraint_count;
    while (k<=vertices) {
        constraint_count = (vertices*(vertices-1))/2 - count + 4*vertices*k + vertices + k + 2;
        std::ofstream outputFile("temp.txt", std::ios::trunc);
        outputFile << "p cnf " + std::to_string(vertices+(vertices+1)*(k+1)) + " " + std::to_string(constraint_count) << std::endl;
        for (int i = 0; i < vertices; i++) {
            for (int j=i+1; j < vertices; j++) {
                if (adjacencyMatrix[i][j]) {
                    // Create a line to write and write the line to the file
                    std::string line1 = "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0\n";
                    outputFile << line1;
                }            
            }
            
        }

        std::string line2;
        std::string line3;
        std::string line4;
        std::string line5;
        int a;
        int b;
        for (int i=0; i<=vertices; i++) {
            for (int j=0; j<=k; j++){
                if (i==0) {
                    line2 = "-" + std::to_string(j+1+vertices) + " 0\n";
                    outputFile << line2;
                }
                else if (j==0) {
                    line2 = std::to_string(i*(k+1)+1+vertices) + " 0\n";
                    outputFile << line2;
                }
                else {
                    a = i*(k+1) + (j+1) + vertices;
                    b = (i-1)*(k+1) + (j+1) + vertices;
                    line2 = std::to_string(a) + " -" + std::to_string(b) + " 0\n";
                    line3 = "-"+std::to_string(b-1) + " -" + std::to_string(i) + " " + std::to_string(a) + " 0\n";
                    line4 = "-"+std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(b-1) + " 0\n";
                    line5 = "-"+std::to_string(a) + " " + std::to_string(b) + " -" + std::to_string(b-1) + " " + std::to_string(i)+" 0\n";
                    outputFile << line2 << line3 << line4 << line5;
                }
            }
        }
        outputFile << std::to_string(vertices+(vertices+1)*(k+1))+" 0\n";
        k++;
        
        // Check if outputFile is satisfiable 
        // if not satisfiable then break
        outputFile.close();
    }
    


    return 0;
}
