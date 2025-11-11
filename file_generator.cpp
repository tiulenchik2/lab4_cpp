#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <random>
#include <format>

void generate_file(const std::string& filename,
const std::vector<double>& probs,
const std::vector<std::string>& actions,
long long total_lines){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(probs.begin(), probs.end());

    std::ofstream outfile(filename);
    if (!outfile){
        std::cerr << "Error opening file " << filename << "\n";
        return;
    }
    for (long long i = 0; i < total_lines; ++i)
    {
        int act_index = dist(gen);
        outfile << actions[act_index]<<"\n";
    }
    std::cout << std::format("Finished {} ({} lines.)", filename, total_lines)<<"\n";
}

int main(){
    const long long NUM_LINES = 1000000;
    const int MAX_THREADS = 3;
    const std::vector<std::string> all_actions = {
        "read 0",
        "write 0 1",
        "read 1",
        "write 1 1",
        "string"
    };
    try{
        // A - "для послідовності дій, частоти якої відповідають умові"
        std::vector<double> probs_A = {10.0, 5.0, 50.0, 10.0, 25.0};
        for (int i = 1; i <= MAX_THREADS; ++i){
            generate_file(std::format("test_A_{}.txt", i), probs_A, all_actions, NUM_LINES);
        }
        // B - "для послідовності дій, де всі частоти рівні"
        std::vector<double> probs_B = {20.0, 20.0, 20.0, 20.0, 20.0};
        for (int i =  1; i <= MAX_THREADS; ++i){
            generate_file(std::format("test_B_{}.txt", i), probs_B, all_actions, NUM_LINES);
        }
        // C - "для послідовності дій, частоти якої дуже сильно не відповідають умові"
        std::vector<double> probs_C = {5.0, 10.0, 5.0, 80.0, 0.0};
        for (int i = 1; i <= MAX_THREADS; ++i){
            generate_file(std::format("test_C_{}.txt", i), probs_C, all_actions, NUM_LINES);
        }
    } catch (const std::exception& e){
        std::cerr<<"\nFatal error! "<<e.what()<<"\n";
        return 1;
    }
    std::cout<<"All files generated."<<"\n";
    return 0;
}