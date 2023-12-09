#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

int main(int argc, char *argv[]) {

    std::ifstream input_file(argv[1]);


    // Process input file (test.graphs) and generate the required output

    // number of vertices and edges of graph fetched from the first line of input graph file
    int vertices, edges;
    std::string first_line;
    std::getline(input_file, first_line);
    std::istringstream iss(first_line);
    iss >> vertices >> edges;
    // std::cout << "vertices: " << vertices << std::endl;
    // std::cout << "edges: " << edges << std::endl;


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
            // std::cout << count << std::endl;
    }
    


    // iterate over all values of k from 1 to vertices
    int k = 1;
    int constraint_count;
    std::string command = "./minisat temp.txt result.txt";
    // now find between k/2 and k-1 (both including)
    int low = 1;    // Lower bound
    int high = vertices;    // Upper bound

    while (low <= high) {
        int mid = (high + low) / 2;
        k = mid;
        constraint_count = (vertices*(vertices-1))/2 - count + 4*vertices*k + vertices + k + 2; //- ((k-1)*k*3)/2 - k;
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
        // std::cout << "hello\n";
        std::string line2;
        std::string line3;
        std::string line4;
        std::string line5;
        // std::string line6;
        int a;
        int b;
        for (int i=0; i<=vertices; i++) {
            for (int j=0; j<=k; j++){
                if (j==0) {
                    line2 = std::to_string(i*(k+1)+1+vertices) + " 0\n";
                    outputFile << line2;
                }
                else if (i==0) {
                    line2 = "-" + std::to_string(j+1+vertices) + " 0\n";
                    outputFile << line2;
                }
                else {
                    a = i*(k+1) + (j+1) + vertices;
                    b = (i-1)*(k+1) + (j+1) + vertices;
                    line2 = std::to_string(a) + " -" + std::to_string(b) + " 0\n";
                    // line6 = std::to_string(a) + " -" + std::to_string(b) + " -" + std::to_string(b-1) + " 0\n";
                    line3 = "-"+std::to_string(b-1) + " -" + std::to_string(i) + " " + std::to_string(a) + " 0\n";
                    line4 = "-"+std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(b-1) + " 0\n";
                    line5 = "-"+std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(i)+" 0\n";
                    outputFile << line5 << line2 << line4 << line3;
                }
                // Insight : the order of clauses matters in minisat :)
            }
        }
        outputFile << std::to_string(vertices+(vertices+1)*(k+1))+" 0\n";
        // std::cout << "\n\n\n\n\nk:" << std::to_string(k) << "\n";
        // k*=2;
        
        outputFile.close();

        // Check if outputFile is satisfiable 
        // if not satisfiable then break
        int returnCode = system(command.c_str());
        std::ifstream result_file("result.txt");
        std::string sat;
        std::string s;
        result_file >> sat;
        if (sat == "UNSAT") {
            high = mid-1;
        }
        else {
            std::ofstream outputFile2(argv[2], std::ios::trunc);
            low = mid+1;
            int c1 = 0;
            for (int i=0; i<vertices ; i++) {
                result_file >> s;
                if (s[0]!='-') {
                    c1++;
                }
            }

            std::ifstream result_file_repeat("result.txt");
            result_file_repeat >> s;
            outputFile2 << "#1\n";
            int c2 = 0;
            for (int i=0; i<vertices ; i++) {
                result_file_repeat >> s;
                if (s[0]!='-') {
                    c2++;
                    if (c1==c2) outputFile2 << s;
                    else outputFile2 << s << " ";
                }
            }
            outputFile2.close();
        }
    }
    return 0;
}
