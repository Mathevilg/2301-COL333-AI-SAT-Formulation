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

    // Fetching the information such as number of vertices, number of edges and k1, k2
    int vertices, edges, k1, k2;
    std::string first_line;
    std::getline(input_file, first_line);
    std::istringstream iss(first_line);
    iss >> vertices >> edges >> k1 >> k2;

    // Construct an adjacency matrix by reading the graph
    std::vector<std::vector<bool> > adjacencyMatrix (vertices, std::vector<bool>(vertices, 0));
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        int row, col;
        iss >> row >> col;
        adjacencyMatrix[row-1][col-1] = 1;
        adjacencyMatrix[col-1][row-1] = 1;
        // std::cout << line << std::endl;
    }

    // number of variables - vertices*2 + (k1+2)*(vertices+1) + (k2+2)*(vertices+1)
    // number of clauses - n + 2*(nC2 - edges) + (k1+1)*n*4 + 2 + (k1+n+1) + (k2+1)*n*4 + 2 + (k2+n+1)

    int no_of_variables = vertices*2 + (k1+2)*(vertices+1) + (k2+2)*(vertices+1);
    int no_of_constraints = vertices + 2*((vertices*(vertices-1))/2 - edges) + (k1+1)*vertices*4 + 2 + (k1+vertices+2) + (k2+1)*vertices*4 + 2 + (k2+vertices+2);
    
    std::ofstream outputFile("test.satinput");
    outputFile << "p cnf " + std::to_string(no_of_variables) + " " + std::to_string(no_of_constraints) << std::endl;

    // adding the clauses ~Ai or ~Bi which ensure that a person is in only 1 agency
    for (int i = 1 ; i <= vertices; i++){
        outputFile << "-" + std::to_string(i) + " -" + std::to_string(i+vertices) + " 0\n";
    }

    // adding clauses which ensure that the people who don't talk to one another are not in the same agency
    for (int i = 0; i < vertices; i++){
        for (int j = 0; j < vertices; j++){
            if (adjacencyMatrix[i][j] == 0){
                outputFile << "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0\n";
                outputFile << "-" + std::to_string(i+vertices+1) + " -" + std::to_string(j+vertices+1) + " 0\n";
            }
        }
    }

    // defining the clauses with respect to s_k,n
    std::string line1;
    std::string line2;
    std::string line3;
    std::string line4;
    int s_k_n;
    int s_k_n_1;
    int s_k_1_n_1;
    for (int k = 0; k <= k1+1; k++){
        for (int n = 0; n <= vertices; n++){
            if (k==0){
                outputFile << std::to_string(2*vertices + n + 1) + " 0\n";
            }
            else if (n == 0){
                outputFile << "-" + std::to_string(2*vertices + k*(vertices+1) + n + 1) + " 0\n";
            }
            else{
                s_k_n = 2*vertices + k*(vertices+1) + n + 1;
                s_k_n_1 = 2*vertices + k*(vertices+1) + n-1 + 1;
                s_k_1_n_1 = 2*vertices + (k-1)*(vertices+1) + n-1 + 1;
                line1 = "-" + std::to_string(s_k_n_1) + " " + std::to_string(s_k_n) + " 0\n";
                line2 = "-" + std::to_string(vertices) + " -" + std::to_string(s_k_1_n_1) + " " + std::to_string(s_k_n) + " 0\n";
                line3 = "-" + std::to_string(s_k_n) + " -" + std::to_string(s_k_n_1) + " " + std::to_string(vertices) + " 0\n";
                line4 = "-" + std::to_string(s_k_n) + " -" + std::to_string(s_k_n_1) + " " + std::to_string(s_k_1_n_1) + " 0\n";
                outputFile << line1 << line2 << line3 << line4;
            }
        }
    }

    for (int k = 0; k <= k2+1; k++){
        for (int n = 0; n <= vertices; n++){
            if (k==0){
                outputFile << std::to_string(2*vertices + (k1+2)*(vertices+1) + n + 1) + " 0\n";
            }
            else if (n == 0){
                outputFile << "-" + std::to_string(2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n + 1) + " 0\n";
            }
            else{
                s_k_n = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n + 1;
                s_k_n_1 = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n-1 + 1;
                s_k_1_n_1 = 2*vertices + (k1+2)*(vertices+1) + (k-1)*(vertices+1) + n-1 + 1;
                line1 = "-" + std::to_string(s_k_n_1) + " " + std::to_string(s_k_n) + " 0\n";
                line2 = "-" + std::to_string(2*vertices) + " -" + std::to_string(s_k_1_n_1) + " " + std::to_string(s_k_n) + " 0\n";
                line3 = "-" + std::to_string(s_k_n) + " -" + std::to_string(s_k_n_1) + " " + std::to_string(2*vertices) + " 0\n";
                line4 = "-" + std::to_string(s_k_n) + " -" + std::to_string(s_k_n_1) + " " + std::to_string(s_k_1_n_1) + " 0\n";
                outputFile << line1 << line2 << line3 << line4;
            }
        }
    }
    outputFile << std::to_string(2*vertices + k1*(vertices+1) + vertices + 1) + " 0\n";
    outputFile << std::to_string(2*vertices + (k1+2)*(vertices+1) + k2*(vertices+1) + vertices + 1) + " 0\n";
    outputFile << "-" + std::to_string(2*vertices + (k1+1)*(vertices+1) + vertices + 1) + " 0/n";
    outputFile << "-" + std::to_string(2*vertices + (k1+2)*(vertices+1) + (k2+1)*(vertices+1) + vertices + 1) + " 0\n";

    outputFile.close();

    
    return 0;
}
