#include "datastructure.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>

enum class Action {READ, WRITE, STRING};
struct Command{
    Action action;
    int field; // 0-1 field, -1 is for string operation
    int value;
};

Command parse_line(const std::string& line){
    std::stringstream ss(line);
    std::string action_type;
    ss >> action_type;

    if (action_type == "read"){
        int field;
        ss >> field;
        return {Action::READ, field, 0};
    }
    if (action_type == "write"){
        int field, value;
        ss >> field >> value;
        return {Action::WRITE, field, value};
    }
    return {Action::STRING, -1, 0};
}

std::vector<Command> load_file_to_memory(const std::string& filename){
    std::vector<Command> commands;
    std::ifstream infile(filename);
    if (!infile){
        std::cerr << std::format("Cannot open file {}.", filename);
        throw std::runtime_error(filename + ": File not found.");
    }
    std::string line;
    commands.reserve(1000000);
    while (std::getline(infile, line)){
        commands.push_back(parse_line(line));
    }
    return commands;
}

void work_func(DataStructure* structure, const std::vector<Command>* commands){
    for (const auto& cmd : *commands){
        switch (cmd.action){
            case Action::READ:
                structure->get_field(cmd.field);
                break;
            case Action::WRITE:
                structure->set_field(cmd.field, cmd.value);
                break;
            case Action::STRING:
                std::string s = std::string(*structure);
                break;
        }
    }
}

double run_test(const std::string& test_type, const std::vector<std::string>& filenames){
    int num_threads = filenames.size();
    DataStructure structure(2);

    std::vector<std::vector<Command>> thread_commands(num_threads);
    for (int i = 0; i < num_threads; ++i){
        thread_commands[i] = load_file_to_memory(filenames[i]);
    }
    
    std::vector<std::thread> threads(num_threads);
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i){
        threads[i] = std::thread(work_func, &structure, &thread_commands[i]);
    }
    for (int i = 0; i < num_threads; ++i){
        threads[i].join();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;
    double time_ms = duration.count();

    std::cout << std::format("TEST {} ({} threads): {} ms", test_type, num_threads, time_ms)<<"\n";
    return time_ms;
}

int main(){
    std::cout<<"Starting benchmarks"<<"\n";

    double results[3][3]; // [A,B,C][1,2,3]
    try {
        std::cout << "\nRunning Test A:" << std::endl;
        results[0][0] = run_test("Type A", {"test_A_1.txt"});
        results[0][1] = run_test("Type A", {"test_A_1.txt", "test_A_2.txt"});
        results[0][2] = run_test("Type A", {"test_A_1.txt", "test_A_2.txt", "test_A_3.txt"});
        
        std::cout << "\nRunning Test B:" << std::endl;
        results[1][0] = run_test("Type B", {"test_B_1.txt"});
        results[1][1] = run_test("Type B", {"test_B_1.txt", "test_B_2.txt"});
        results[1][2] = run_test("Type B", {"test_B_1.txt", "test_B_2.txt", "test_B_3.txt"});

        std::cout << "\nRunning Test C:" << std::endl;
        results[2][0] = run_test("Type C", {"test_C_1.txt"});
        results[2][1] = run_test("Type C", {"test_C_1.txt", "test_C_2.txt"});
        results[2][2] = run_test("Type C", {"test_C_1.txt", "test_C_2.txt", "test_C_3.txt"});

    } catch (const std::exception& e) {
        std::cerr << "\n*** A FATAL ERROR OCCURRED: " << e.what() << " ***" << std::endl;
        std::cerr << "Please ensure test files (test_A_1.txt, etc.) exist." << std::endl;
        return 1; // Повертаємо код помилки
    }

    std::cout << "\nBenchmarks finished" << std::endl;
    return 0;
}