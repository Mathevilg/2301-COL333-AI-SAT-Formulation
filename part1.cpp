#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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
    }

    int no_of_variables = vertices*2 + (k1+2)*(vertices+1) + (k2+2)*(vertices+1);
    // int no_of_constraints = vertices + 2*((vertices*(vertices-1))/2 - edges) + ((k1+1)*vertices*4 + 2 + (k1+vertices+2)) + ((k2+1)*vertices*4 + 2 + (k2+vertices+2));

    std::ofstream outputFile("test1.satinput");
    std::vector<std::string> clauses;
    // outputFile << "p cnf " + std::to_string(no_of_variables) + " " + std::to_string(no_of_constraints) << std::endl;
    // outputFile << std::to_string(no_of_variables) + " " + std::to_string(no_of_constraints) << std::endl;

    // outputFile << "p cnf " + std::to_string(no_of_variables) + " 2227168" << std::endl;
    
    // adding the clauses (~Ai or ~Bi) which ensure that a person is in only 1 agency
    std::string clause_type1;
    for (int i = 1 ; i <= vertices; i++){
        clause_type1 =  "-" + std::to_string(i) + " -" + std::to_string(i+vertices) + " 0";
        clauses.push_back(clause_type1);
    }

    // adding clauses which ensure that the people who don't talk to one another are not in the same agency
    std::string clause_type2;
    for (int i = 0; i < vertices; i++){
        for (int j = i+1; j < vertices; j++){
            if (adjacencyMatrix[i][j] == 0){
                clause_type2 =  "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0";
                clauses.push_back(clause_type2);
                clause_type2 = "-" + std::to_string(i+vertices+1) + " -" + std::to_string(j+vertices+1) + " 0";
                clauses.push_back(clause_type2);
            }
        }
    }

    // defining the clauses with respect to s_k,n where s_k,n says that atleast k variables are true out of n
    std::string line1,line2,line3,line4,clause_type3;
    int s_k_n, s_k_n_1, s_k_1_n_1;
    std::vector<int> False_literals, True_literals;

    clause_type3 = std::to_string(2*vertices + k1*(vertices+1) + vertices + 1) + " 0";
    clauses.push_back(clause_type3);
    True_literals.push_back(2*vertices + k1*(vertices+1) + vertices + 1);

    clause_type3 = std::to_string(2*vertices + (k1+2)*(vertices+1) + k2*(vertices+1) + vertices + 1) + " 0";
    clauses.push_back(clause_type3);
    True_literals.push_back(2*vertices + (k1+2)*(vertices+1) + k2*(vertices+1) + vertices + 1);

    clause_type3 = "-" + std::to_string(2*vertices + (k1+1)*(vertices+1) + vertices + 1) + " 0";
    clauses.push_back(clause_type3);
    False_literals.push_back(2*vertices + (k1+1)*(vertices+1) + vertices + 1);

    clause_type3 = "-" + std::to_string(2*vertices + (k1+2)*(vertices+1) + (k2+1)*(vertices+1) + vertices + 1) + " 0";
    clauses.push_back(clause_type3);
    False_literals.push_back(2*vertices + (k1+2)*(vertices+1) + (k2+1)*(vertices+1) + vertices + 1);

    std::cout<<"starting wiht k1"<<std::endl;

    for (int k = 0; k <= k1+1; k++){
        std::cout<<"k1: "<<k<<std::endl;
        for (int n = 0; n <= vertices; n++){
            if (k==0){
                int literal = 2*vertices + n + 1;
                clause_type3 =  std::to_string(literal) + " 0";
                clauses.push_back(clause_type3);
                True_literals.push_back(literal);
            }
            else if (n == 0){
                int literal =2*vertices + k*(vertices+1) + n + 1;
                clause_type3 = "-" + std::to_string(2*vertices + k*(vertices+1) + n + 1) + " 0";
                clauses.push_back(clause_type3);
                False_literals.push_back(literal);
            }
            else if (k > n){
                int literal = 2*vertices + k*(vertices+1) + n + 1;
                clause_type3 = "-" + std::to_string(2*vertices + k*(vertices+1) + n + 1) + " 0";
                clauses.push_back(clause_type3);
                False_literals.push_back(literal);
            }
            else if (k == n){
                s_k_n = 2*vertices + k*(vertices+1) + n + 1;
                s_k_1_n_1 = 2*vertices + (k-1)*(vertices+1) + n-1 + 1;

                line3 = "-" + std::to_string(s_k_n) +  " " + std::to_string(n) + " 0";

                if (find(True_literals.begin(), True_literals.end(), s_k_1_n_1) != True_literals.end()){
                    line2 = "-" + std::to_string(n) + " " + std::to_string(s_k_n) + " 0";
                    clauses.push_back(line2);
                    clauses.push_back(line3);
                }
                else if (find(False_literals.begin(), False_literals.end(), s_k_1_n_1) != False_literals.end()){
                    line4 = "-" + std::to_string(s_k_n) +  " 0";
                    False_literals.push_back(s_k_n);
                    clauses.push_back(line4);
                }
                else{
                    line2 = "-" + std::to_string(n) + " " + std::to_string(s_k_n) + " -" + std::to_string(s_k_1_n_1) + " 0";
                    line4 = "-" + std::to_string(s_k_n) + " " + std::to_string(s_k_1_n_1) + " 0";
                    clauses.push_back(line2);
                    clauses.push_back(line4);
                    clauses.push_back(line3);
                }   
            }
            else{
                s_k_n = 2*vertices + k*(vertices+1) + n + 1;
                s_k_n_1 = 2*vertices + k*(vertices+1) + n-1 + 1;
                s_k_1_n_1 = 2*vertices + (k-1)*(vertices+1) + n-1 + 1;
                
                bool line4_flag = true;
                bool s_k_n_flag1 = false;
                bool s_k_n_flag2 = false;
                line4 = "-" + std::to_string(s_k_n);
                
                if (find(True_literals.begin(), True_literals.end(), s_k_n_1) != True_literals.end()){
                    line1 = std::to_string(s_k_n) + " 0";
                    clauses.push_back(line1);
                    True_literals.push_back(s_k_n);
                    line4_flag = false;
                }
                else if (find(False_literals.begin(), False_literals.end(), s_k_n_1) != False_literals.end()){
                    line3 = "-" + std::to_string(s_k_n) + " " + std::to_string(n) + " 0";
                    clauses.push_back(line3);
                    s_k_n_flag1 = true;
                }
                else{
                    line1 = "-" + std::to_string(s_k_n_1) + " " + std::to_string(s_k_n) + " 0";
                    line3 = "-" + std::to_string(s_k_n) + " " + std::to_string(s_k_n_1) + " " + std::to_string(n) + " 0";
                    line4 +=  " " + std::to_string(s_k_n_1);
                    clauses.push_back(line1);
                    clauses.push_back(line3);
                }

                if (find(True_literals.begin(), True_literals.end(), s_k_1_n_1) != True_literals.end()){
                    line2 = "-" + std::to_string(n) + " " + std::to_string(s_k_n) + " 0";
                    clauses.push_back(line2);
                    line4_flag = false;
                }
                else if (find(False_literals.begin(), False_literals.end(), s_k_1_n_1) != False_literals.end()){
                    s_k_n_flag2 = true;
                    continue;
                }
                else{
                    line4 +=  " " + std::to_string(s_k_1_n_1);
                    line2 = "-" + std::to_string(n) + " -" + std::to_string(s_k_1_n_1) + " " + std::to_string(s_k_n) + " 0";
                    clauses.push_back(line2);
                }
                if (line4_flag){
                    if (s_k_n_flag1 && s_k_n_flag2){
                        line4 += " 0";
                        clauses.push_back(line4);
                        False_literals.push_back(s_k_n);
                    }
                    else{
                        line4 += " 0";
                        clauses.push_back(line4);
                    }
                }
            }
        }
    }

    std::cout<<"finished with k1 "<<std::endl;

    for (int k = 0; k <= k2+1; k++){
        std::cout<<"k2: "<<k<<std::endl;
        for (int n = 0; n <= vertices; n++){
            if (k==0){
                int literal = 2*vertices + (k1+2)*(vertices+1) + n + 1;
                clause_type3 =  std::to_string(literal) + " 0";
                // std::cout<<clause_type3;
                clauses.push_back(clause_type3);
                True_literals.push_back(literal);
            }
            else if (n == 0){
                int literal = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n + 1;
                clause_type3 = "-" + std::to_string(literal) + " 0";
                clauses.push_back(clause_type3);
                False_literals.push_back(literal);
            }
            else if (k > n){
                int literal = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n + 1;
                clause_type3 = "-" + std::to_string(literal) + " 0";
                clauses.push_back(clause_type3);
                False_literals.push_back(literal);
            }
            else if (k == n){
                s_k_n = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n + 1;
                s_k_1_n_1 = 2*vertices + (k1+2)*(vertices+1) + (k-1)*(vertices+1) + n-1 + 1;

                line3 = "-" + std::to_string(s_k_n) +  " " + std::to_string(vertices + n) + " 0";

                if (find(True_literals.begin(), True_literals.end(), s_k_1_n_1) != True_literals.end()){
                    line2 = "-" + std::to_string(vertices + n) + " " + std::to_string(s_k_n) + " 0";
                    clauses.push_back(line2);
                    clauses.push_back(line3);
                }
                else if (find(False_literals.begin(), False_literals.end(), s_k_1_n_1) != False_literals.end()){
                    line4 = "-" + std::to_string(s_k_n) +  " 0";
                    False_literals.push_back(s_k_n);
                    clauses.push_back(line4);
                }
                else{
                    line2 = "-" + std::to_string(vertices + n) + " " + std::to_string(s_k_n) + " -" + std::to_string(s_k_1_n_1) + " 0";
                    line4 = "-" + std::to_string(s_k_n) + " " + std::to_string(s_k_1_n_1) + " 0";
                    clauses.push_back(line2);
                    clauses.push_back(line4);
                    clauses.push_back(line3);
                }   
            }
            else{
                s_k_n = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n + 1;
                s_k_n_1 = 2*vertices + (k1+2)*(vertices+1) + k*(vertices+1) + n-1 + 1;
                s_k_1_n_1 = 2*vertices + (k1+2)*(vertices+1) + (k-1)*(vertices+1) + n-1 + 1;

                bool line4_flag = true;
                bool s_k_n_flag1 = false;
                bool s_k_n_flag2 = false;
                line4 = "-" + std::to_string(s_k_n);
                
                if (find(True_literals.begin(), True_literals.end(), s_k_n_1) != True_literals.end()){
                    line1 = std::to_string(s_k_n) + " 0";
                    clauses.push_back(line1);
                    True_literals.push_back(s_k_n);
                    line4_flag = false;
                }
                else if (find(False_literals.begin(), False_literals.end(), s_k_n_1) != False_literals.end()){
                    line3 = "-" + std::to_string(s_k_n) + " " + std::to_string(vertices + n) + " 0";
                    clauses.push_back(line3);
                    s_k_n_flag1 = true;
                }
                else{
                    line1 = "-" + std::to_string(s_k_n_1) + " " + std::to_string(s_k_n) + " 0";
                    line3 = "-" + std::to_string(s_k_n) + " " + std::to_string(s_k_n_1) + " " + std::to_string(vertices + n) + " 0";
                    line4 +=  " " + std::to_string(s_k_n_1);
                    clauses.push_back(line1);
                    clauses.push_back(line3);
                }

                if (find(True_literals.begin(), True_literals.end(), s_k_1_n_1) != True_literals.end()){
                    line2 = "-" + std::to_string(vertices + n) + " " + std::to_string(s_k_n) + " 0";
                    clauses.push_back(line2);
                    line4_flag = false;
                }
                else if (find(False_literals.begin(), False_literals.end(), s_k_1_n_1) != False_literals.end()){
                    s_k_n_flag2 = true;
                    continue;
                }
                else{
                    line4 +=  " " + std::to_string(s_k_1_n_1);
                    line2 = "-" + std::to_string(vertices + n) + " -" + std::to_string(s_k_1_n_1) + " " + std::to_string(s_k_n) + " 0";
                    clauses.push_back(line2);
                }
                if (line4_flag){
                    if (s_k_n_flag1 && s_k_n_flag2){
                        line4 += " 0";
                        clauses.push_back(line4);
                        False_literals.push_back(s_k_n);
                    }
                    else{
                        line4 += " 0";
                        clauses.push_back(line4);
                    }
                }
            }
        }
    }

    std::cout<<"checked all clauses...writing it "<<std::endl;

    outputFile << "p cnf " + std::to_string(no_of_variables) + " " + std::to_string(clauses.size()) + "\n";
    for (int i = 0; i < clauses.size(); i++){
        outputFile << clauses[i] << std::endl;
    }

    // std::cout<<"true literals\n";
    // for (int i = 0; i < True_literals.size(); i++){
    //     std::cout<<True_literals[i]<<" ";
    // }
    // std::cout<<"\n";
    // std::cout<<"false literals\n";
    // for (int i = 0; i < False_literals.size(); i++){
    //     std::cout<<False_literals[i]<<" ";
    // }
    
    outputFile.close();

    // process_unit_literals("test1.satinput");

    return 0;
}
