#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Task
{
    int id;
    string title;
    bool done = false;
    int priority;
    string deadline;
};

void add(string args,vector<Task>&tasks){
    // Title
    size_t first_quote = args.find('\"');
    if (first_quote == string::npos){
        cout << "Ошибка: Название задачи должно быть в кавычках" << '\n';
        return;
    }
    size_t second_quote = args.find('\"',first_quote + 1);
    if(second_quote == string::npos){
        cout << "Ошибка: Название должно заканчиваться кавычкой" << '\n';
        return;
    }
    size_t len = second_quote - first_quote - 1;
    string title = args.substr(first_quote + 1,len);

    // Приоритет
    size_t p_pos = args.find("-p ");
    if (p_pos == string::npos) {
        cout << "Ошибка: Пропущено '-p' " << '\n';
        return;
    }
    size_t p_end = args.find(' ', p_pos + 3); 
    string p_str;
    if (p_end == string::npos) {
        p_str = args.substr(p_pos + 3); 
    } else {
        size_t p_len = p_end - (p_pos + 3);
        p_str = args.substr(p_pos + 3, p_len);
    }
    int priority = stoi(p_str);

    // Дедлайн
    size_t d_pos = args.find("-d ");
    if (d_pos == string::npos) {
        cout << "Ошибка: Пропущено '-d' "<< '\n';
        return;
    }
    size_t d_end = args.find(' ', d_pos + 3); 
    string d_str;
    if (d_end == string::npos) {
        d_str = args.substr(d_pos + 3); 
    } else {
        size_t d_len = d_end - (d_pos + 3);
        d_str = args.substr(d_pos + 3, d_len);
    }
    string deadline = d_str;

    // ID
    int new_id = 1;
    if (!tasks.empty()) {
        new_id = tasks.back().id + 1;
    }

    // Структура
    Task new_task;
    new_task.id = new_id;
    new_task.title = title;
    new_task.done = false;
    new_task.priority = priority;
    new_task.deadline = deadline;

    tasks.push_back(new_task);
    cout << "Добавлена задача: " << "\n";
    cout << "ID: " << new_id << '\n';
    cout << "Название: " << title << "\n"; 
    cout << "Приоритет: " << priority << "\n";
    cout << "Дедлайн: " << deadline << "\n";
}

void list(const vector<Task>& tasks){
    if (tasks.empty()){
        cout << "Список задач пуст" << '\n';
        return;
    }
    for (int i = 0; i < tasks.size(); i++) {
        cout << "ID: " << tasks[i].id << " | ";
        if (tasks[i].done == true){
            cout << "done | ";
        } 
        else{
            cout << "not done | ";
        }
        cout << tasks[i].title << " | Приоритет: " << tasks[i].priority << " | ";
        if (tasks[i].deadline != "none"){
            cout << "Дедлайн: " << tasks[i].deadline << '\n'; 
        }
        else{
            cout << "Без дедлайна" << '\n';
        }
    }
}

void Done(string args, vector<Task>& tasks){
    if (args.empty()){
        cout << "Ошибка: Укажите номер задачи" << "\n";
        return;
    }
    int target_id = stoi(args); 
    bool found = false;
    for (int i = 0; i < tasks.size(); i++){
        if (tasks[i].id == target_id){
            tasks[i].done = true;
            cout << "Задача с ID " << target_id << " отмечена как выполненная " << "\n";
            found = true;
            break;
        }
    }
    if (found == false){
        cout << "Ошибка: Задача с таким номером не найдена" << "\n";
    }
}

void Undone(string args, vector<Task>& tasks){
    if (args.empty()){
        cout << "Ошибка: Укажите номер задачи\n";
        return;
    }
    int target_id = stoi(args); 
    bool found = false;
    for (int i = 0; i < tasks.size(); i++){
        if (tasks[i].id == target_id){
            tasks[i].done = false;
            cout << "Задача с ID " << target_id << " отмечена как невыполненная "<< "\n";
            found = true;
            break;
        }
    }
    if (found == false){
        cout << "Ошибка: Задача с таким номером не найдена " << "\n";
    }
}

void delete_task(string args, vector<Task>& tasks){
    if (args.empty()){
        cout << "Ошибка: Укажите номер задачи " << "\n";
        return;
    }
    int target_id = stoi(args);
    bool found = false;
    for (int i = 0; i < tasks.size(); i++){
        if (tasks[i].id == target_id){
            tasks.erase(tasks.begin() + i); 
            cout << "Задача с ID " << target_id << " удалена" << "\n";
            found = true;
            break;
        }
    }
    if (found == false){
        cout << "Ошибка: Задача с таким номером не найдена "<< "\n";
    }
}

void clear(vector<Task>& tasks){
    cout << "Удалить все выполненные задачи? (y/n)";
    string answer;
    getline(cin, answer);
    if (answer == "y" || answer == "Y"){
        int count = 0;
        for (int i = tasks.size() - 1; i >= 0; i--){
            if (tasks[i].done == true){
                tasks.erase(tasks.begin() + i);
                count++;
            }
        }
        cout << "Очистка завершена. Удалено задач: " << count << "\n";
    }
    else{
        cout << "Очистка отменена." << "\n";
    }
}

void save_file(const vector<Task>& tasks){
    ofstream file("tasks.txt");
    if (file.is_open() == false){
        cout << "Ошибка: не удалось сохранить файл" << "\n";
        return;
    }
    for (int i = 0; i < tasks.size(); i++){
        file << tasks[i].id << "|";
        file << tasks[i].title << "|";
        if (tasks[i].done == true){
            file << "1|"; 
        }
        else{
            file << "0|";
        }
        file << tasks[i].priority << "|";
        file << tasks[i].deadline << "\n";
    }
    file.close();
}

void load_file(vector<Task>& tasks){
    ifstream file("tasks.txt"); 
    if (file.is_open() == false){
        return; 
    }
    string line;
    while (getline(file, line)){
        if (line.empty()){
            continue;
        }
        Task temp_task;

        // ID
        size_t pos1 = line.find('|');
        string id_str = line.substr(0, pos1);
        temp_task.id = stoi(id_str);

        // Title
        size_t pos2 = line.find('|', pos1 + 1);
        size_t title_len = pos2 - pos1 - 1;
        temp_task.title = line.substr(pos1 + 1, title_len);

        // status
        size_t pos3 = line.find('|', pos2 + 1);
        size_t done_len = pos3 - pos2 - 1;
        string done_str = line.substr(pos2 + 1, done_len);
        if (done_str == "1"){
            temp_task.done = true;
        }
        else{
            temp_task.done = false;
        }

        // priority
        size_t pos4 = line.find('|', pos3 + 1);
        size_t prior_len = pos4 - pos3 - 1;
        string prior_str = line.substr(pos3 + 1, prior_len);
        temp_task.priority = stoi(prior_str);

        // deadline
        string d_str = line.substr(pos4 + 1);
        temp_task.deadline = d_str;

        tasks.push_back(temp_task);
    }
    file.close();
}

void help(){
    cout << "\n" << " --- СПИСОК КОМАНД --- " << "\n";
    cout << "add \"название\" -p приоритет(число) -d [дата/none] - добавить задачу "<< "\n";
    cout << "list                                      - вывести все задачи" << "\n";
    cout << "done [id]                                 - отметить задачу выполненной "<< "\n";
    cout << "undone [id]                               - отметить задачу невыполненной " << "\n";
    cout << "delete [id]                               - удалить задачу" << "\n";
    cout << "clear                                     - удалить все выполненные задачи" << "\n";
    cout << "exit                                      - выйти из программы" << "\n";
    cout << "help                                      - справка " << "\n";
    cout << "----------------" << "\n";
}

int main(){
    system("chcp 65001 > nul"); // Рус яз в консоли
    system("cls");
    vector<Task> tasks;
    load_file(tasks);
    help();
    string input,command;
    while (true){
        system("cls");
        help();
        cout << ">>> ";
        getline(cin,input);
        if (input.empty()){
            continue;
        }
        size_t space_pos = input.find(' ');
        if(space_pos == string::npos){
            command  = input;
        }
        else{
            command = input.substr(0,space_pos);
        }
        if (command == "exit"){
            break;
        }
        else if(command == "add"){
            if (space_pos != string::npos){
                string args = input.substr(space_pos + 1);
                add(args,tasks);
                save_file(tasks);
                cout << "Нажмите enter для продолжения ";
                cin.get();
            }
            else{
                cout << "Ошибка: Нету аргументов для добавлении команды" << "\n";
            }
        }
        else if (command == "list"){
            list(tasks);
            cout << "Нажмите enter для продолжения ";
            cin.get();
        }
        else if (command == "done"){
            if (space_pos != string::npos){
                string args = input.substr(space_pos + 1);
                Done(args, tasks);
                save_file(tasks);
                cout << "Нажмите enter для продолжения ";
                cin.get();
            }
            else{
                cout << "Ошибка: Укажите номер задачи "<< "\n";
            }
        }
        else if (command == "undone"){
            if (space_pos != string::npos){
                string args = input.substr(space_pos + 1);
                Undone(args, tasks);
                save_file(tasks);
                cout << "Нажмите enter для продолжения ";
                cin.get();
            }
            else{
                cout << "Ошибка: Укажите номер задачи" << "\n";
            }
        }
        else if (command == "delete") {
            if (space_pos != string::npos){
                string args = input.substr(space_pos + 1);
                delete_task(args, tasks);
                save_file(tasks);
                cout << "Нажмите enter для продолжения ";
                cin.get();
            }
            else{
                cout << "Ошибка: Укажите номер задачи "<< "\n";
            }
        }
        else if (command == "clear"){
            clear(tasks);
            save_file(tasks);
            cout << "Нажмите enter для продолжения ";
            cin.get();
        }
        else if (command == "help"){
            help();
        }
        else{
            cout << "Ошибка: Неизвестная команда" << "\n"; 
        }
    }
}