#include<bits/stdc++.h>
using namespace std;

struct LinkedinUser{
    string username;
    vector<string> skills;
    set<string> connections;
    set<string> languages;
    unordered_map<string,int> education;
    unordered_map<string,int> experience;

    //Empty constructor
    LinkedinUser(){}

    //constructor
    LinkedinUser(string username,vector<string> skills,set<string> languages,unordered_map<string,int> education, unordered_map<string,int> experience){
        this -> username = username;
        this -> skills = skills;
        this -> languages = languages;
        this -> education = education;
        this -> experience = experience;
    }
    //For printing the profile
    void print(){
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<endl;
        cout<<"User Name: "<<this->username<< endl;
        cout<<endl;
        cout<<"Total connections: "<<connections.size();
        cout<<endl;
        cout<<"Connections: ";
        for(auto it: connections){
            cout<<it<<" ";
        }
        cout<<endl;
        cout<<endl;
        cout<<"Skills: ";
        for(auto it: skills){
            cout<<it<<" ";
        }
        cout<<endl;
        cout<<"Total Skills: "<<skills.size()<<endl;
        cout<<endl;
        cout<<"Languages known: ";
        for(auto it: languages){
            cout<<it<<" ";
        }
        cout<<endl;
        cout<<endl;
        cout<<"Education: "<<endl;
        for(auto it: education){
            cout<<it.first<<"           "<<it.second<<endl;
        }
        cout<<endl;
        cout<<endl;
        cout<<"Experience: "<<endl;
        for(auto it: experience){
            cout<<it.first<<"         "<<it.second<<endl;
        }


        cout<<endl;
    }
};

//map stores the username mapped to the user's linkedinUser
map<string,LinkedinUser*> linkedin_user;

void addEdge(string source_username, string destination_username){

    //Establishing an UNDIRECTED EDGE between source_username & destination_username.

    linkedin_user[source_username]->connections.insert(destination_username);
    linkedin_user[destination_username]->connections.insert(source_username);
}

void printGraph(){
    cout<<"The adjacency List: "<<endl;
    cout<<endl;
    for(auto user: linkedin_user){
        cout<<"Username: "<<user.first<<endl;
        cout<<"Connections are: ";
        cout<<endl;
        int cnt = 1;
        for(auto connection: user.second->connections){
            cout<<cnt<<"."<<connection<<endl;
            cnt++;
        }
        cout<<endl;
    }
}
//For clearing memory
void clearMemory(){
    for(auto user : linkedin_user){
        delete user.second;
    }
}

//BREADTH FIRST SEARCH 

void BFS(string initial_node){
    vector<string> bfsjourney; //To record bfs journey.
  //This is going to contain the usernames in the order they need to processed vis the BFS.
    queue<string> username_q;
  //Going to contain our visited node information.
    unordered_set<string> visited_connections;

    map<string,int> user_to_level;
    int current_level = 0;
    //push the initial node.
    username_q.push(initial_node);
    user_to_level[initial_node] = current_level;
    //Now i need to mark the initail node to be visited.
    visited_connections.insert(initial_node);

    while(!username_q.empty()){
    //we need to get the front element from the queue, and remove it from the queue as well
        string front_name = username_q.front();
        username_q.pop();
        
        //Get the user object associated with the front username
        LinkedinUser* front_user = linkedin_user[front_name];
        current_level = user_to_level[front_name];

        // //processing of the node
        front_user->print();
        bfsjourney.push_back(front_name);
        for(string neighbour_username : front_user->connections){
            if(visited_connections.find(neighbour_username) == visited_connections.end()){
                username_q.push(neighbour_username);
                user_to_level[neighbour_username] = current_level + 1;
                visited_connections.insert(neighbour_username);
            }
        }
    }
    
    cout<<"--------------------------------------------------------------------"<<endl;
    cout<<"--------------------------------------------------------------------"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"*******************  Level of connections with the help of BFS  ******************* "<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"All the users are at the following level from "<<initial_node<<":"<<endl;
    cout<<endl;
    for(auto user: user_to_level){
        if(user.second == 0) continue;
        if(user.second>3){
            cout<<user.first<<" is at other level "<<endl;
        }
        else{
        cout<<user.first<<" is at "<<user.second<<" level" <<endl;
        }
    }
    cout<<endl;
    cout<<endl;
    cout<<"The BFS journey: "<<endl;
    for(auto it: bfsjourney){
            cout<<it<<"-->";
        }


}

void dfsFindMutualConnections(string user1, string user2, set<string>& visited, set<string>& mutualConnections) {
    visited.insert(user1);

    // Check if user2 is a connection of user1
    if (linkedin_user[user1]->connections.count(user2)) {
        mutualConnections.insert(user1);
        return;
    }

    // Recursively visit unvisited connections of user1
    for (const string& connection : linkedin_user[user1]->connections) {
        if (visited.find(connection) == visited.end()) {
            dfsFindMutualConnections(connection, user2, visited, mutualConnections);
        }
    }
}

// Function to find mutual connections between two users
set<string> findMutualConnections(string user1, string user2) {
    set<string> visited;
    set<string> mutualConnections;

    dfsFindMutualConnections(user1, user2, visited, mutualConnections);

    return mutualConnections;
}


//Suggestion feature

// Function to calculate the number of shared skills between two users
int sharedSkills(const LinkedinUser* user1, const LinkedinUser* user2) {
    vector<string> shared;
    set_intersection(user1->skills.begin(), user1->skills.end(),
                     user2->skills.begin(), user2->skills.end(),
                     back_inserter(shared));
    return shared.size();
}

// Function to suggest new connections based on shared skills
vector<string> suggestConnections(const LinkedinUser* currentUser, const map<string, LinkedinUser*>& allUsers) {
    priority_queue<pair<int, string>> suggestedUsers;  // Min heap based on shared skills

    for (const auto& user : allUsers) {
        if (user.first != currentUser->username) {
            int shared = sharedSkills(currentUser, user.second);
            suggestedUsers.push({shared, user.first});
        }
    }

    vector<string> suggestedConnections;
    while (!suggestedUsers.empty()) {
        suggestedConnections.push_back(suggestedUsers.top().second);
        suggestedUsers.pop();
    }

    return suggestedConnections;
}



int main(){
    linkedin_user["Amitabh"] = new LinkedinUser("Amitabh",{"C++","DSA","React","Python","JavaScript"},{"English","Hindi","Marathi"},{{"High school", 2019}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Bikas"] = new LinkedinUser("Bikas",{"Java","DSA"},{"English","Hindi","Marathi"},{{"High school", 2020}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Vaibhav"] = new LinkedinUser("Vaibhav",{"JavaScript","Python"},{"English","Hindi"},{{"High school", 2020}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Sparsh"] = new LinkedinUser("Sparsh",{"MongoDB","Python","JavaScript"},{"English","Hindi","Marwadi"},{{"High school", 2019}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Onkar"] = new LinkedinUser("Onkar",{"AngularJS","Flutter"},{"English","Hindi","Marathi"},{{"High school", 2020}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Siddharth"] = new LinkedinUser("Siddharth",{"GitHub","DSA"},{"English","Hindi"},{{"High school", 2019}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Ashish"] = new LinkedinUser("Ashish",{"C++","Git"},{"English","Hindi","Marathi"},{{"High school", 2020}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Tushar"] = new LinkedinUser("Tushar",{"JavaScript","DSA"},{"English","Hindi","Marwadi"},{{"High school", 2019}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Tanish"] = new LinkedinUser("Tanish",{"C++","NodeJS"},{"English","Hindi","Marwadi"},{{"High school", 2020}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    linkedin_user["Kishan"] = new LinkedinUser("Kishan",{"ML","ExpressJS"},{"English","Hindi","Marwadi"},{{"High school", 2019}, {"Bachelor's", 2026}},{{"Software Engineer", 2015}, {"Project Manager", 2020}});

    //*********Adding EDGES***********

    addEdge("Amitabh","Vaibhav");
    addEdge("Amitabh","Siddharth");
    addEdge("Amitabh","Bikas");
    addEdge("Vaibhav","Sparsh");
    addEdge("Vaibhav","Onkar");
    addEdge("Bikas","Ashish");
    addEdge("Bikas","Tushar");
    addEdge("Tushar","Tanish");
    addEdge("Tanish","Kishan");
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"****************************** Welcome to LinkedIn *********************************"<<endl;
    cout<<endl;
    cout<<endl;


    //Finding the level of connections between users.
    BFS("Amitabh");
    cout<<endl;
    cout<<endl;
    cout<<endl;


    //Finding mutual connections between user using DFS.

    string user1 = "Onkar";
    string user2 = "Amitabh";
    set<string> mutualConnections = findMutualConnections(user1, user2);
     if (mutualConnections.empty()) {
        cout << "No mutual connections found between " << user1 << " and " << user2 << "." << endl;
    } else {
        cout << "Mutual Connections between " << user1 << " and " << user2 << ": ";
        for (const string& connection : mutualConnections) {
            cout << connection;
        }
        cout << endl;
    }
    
    cout<<endl;

    cout<<"Suggestions: "<<endl;

    //Suggestion feature using prims algorithm

    string currentUserName = "Amitabh";
    LinkedinUser* currentUser = linkedin_user[currentUserName];

    vector<string> suggested = suggestConnections(currentUser, linkedin_user);

    cout << "Suggested connections for " << currentUser->username << ":" << endl;
    for (const string& connection : suggested) {
        cout << connection << endl;
    }

    //for clearing the memory
    clearMemory();

    return 0;
}