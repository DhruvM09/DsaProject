#include <bits/stdc++.h>
using namespace std;

class contactNode{
    public:
    string Name;
    long long int number;
    string email;
    contactNode * left;
    contactNode * right;
    contactNode(string name , long long int num){
        Name = name;
        number = num;
        email = "";
        left = nullptr;
        right = nullptr;
    }
    contactNode(string name , long long int num , string mail){
        Name = name;
        number = num;
        email = mail;
        right = nullptr;
        left = nullptr;
    }
};
void printContact(contactNode * &node){
    cout << "Contact Info:";
    cout << "Name:"<< node->Name << '\n';
    cout << "Contact Number:" << node->number << "\n";
    cout << "Email:" << node->email<<'\n';
}
class ConatactBinarySearchTree{
    public:
    contactNode * head;


    ConatactBinarySearchTree(){
        head = nullptr;
    }
    void insert(contactNode * &newNode){
        contactNode * current = head;
        if(head == nullptr){
            head = newNode;
            return;
        }
        contactNode * parent = nullptr;
        while (current != nullptr)
        {
            parent = current;
            if(current->Name >  newNode->Name){
                current = current->left;
            }
            else if(current->Name < newNode->Name){
                current = current->right;
            }
            else{
                current = current->right;
            }
        }
        if(newNode->Name > parent->Name)parent->right = newNode;
        else if(newNode->Name < parent->Name)parent->left = newNode;
        
        cout << "insertion successful" << '\n';
    }

    contactNode * searchByName(string key){
        contactNode * currentNode = head; 
        while (currentNode != nullptr)
        {
            if(currentNode->Name < key){
                currentNode = currentNode->right;
            }
            else if(currentNode->Name > key){
                currentNode = currentNode->left;
            }else{
                break;
            }
        }
        if(currentNode->Name == key){
            return currentNode;
        }
        else {
            cout << "not found"<< '\n';
            return nullptr;
        }
        
    }
    void dfs(contactNode *current){
        cout << current->Name << " ";
        if(current->left != nullptr)dfs(current->left);
        if(current->right != nullptr)dfs(current->right); 
    }
};
int main(){ 

    ConatactBinarySearchTree ContactTree;
    //main loop
   bool Running = true;
   while (Running)
   {
    int choice ;
    cout << "1.create new contact"<<'\n';
    cout << "2.edit existing contact"<<'\n';
    cout << "3.search contact" << '\n';
    cout << "4.exit" << "\n";
    
    cout << "Enter your choice:";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        long long int num;
        string n;
        cout << "enter name:";
        cin >> n;
        cout << "enter number:";
        cin >> num;
        contactNode * node = new contactNode(n,num);
        ContactTree.insert(node);
        break;
    }
    case 2:
    {
        cout << "Enter which contact to edit";
        string n;cin >> n;
        contactNode * requiredNode = ContactTree.searchByName(n);
        if(requiredNode != nullptr){
            int c;
            cout << "1.Change Name" << '\n';
            cout << "2.Change Number" << '\n';
            cout << "3.Change Email" << '\n';
            cout << "Selec what do you want to change?:";
            cin >> c;
            switch (c)
            {
            case 1:
            {
                cout << "Enter New Name:";
                string newName;cin>>newName;
                requiredNode->Name = newName;
                break;
            }
            case 2:
            {
                cout << "Enter New Numeber:";
                long long int newNumber;cin >> newNumber;
                requiredNode->number = newNumber;
                break;
            } 
            case 3:
            {
                cout << "Enter New Email:";
                string newMail;cin>>newMail;
                requiredNode->email = newMail;
                break;
            }
            default:
                cout << "Invalid choice" << '\n';
                break;
            }
        }
        break;
    }

    case 3:
    {
        string n;
        cout << "Enter Name:";
        cin >> n;
        contactNode * requiredNode = ContactTree.searchByName(n);
        if(requiredNode != nullptr)printContact(requiredNode);

        break;
    }
    case 4:
        Running = false;
        break; 
    default:
        cout << "invalid option" << '\n';
        break;
    }
    
   }
}