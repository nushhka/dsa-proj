#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

class PhoneRecord {
private:
    string name;
    string organisation;
    vector<string> phoneNumbers;

public:
    PhoneRecord(const string& n, const string& org, const vector<string>& numbers)
        : name(n), organisation(org), phoneNumbers(numbers) {}

    // Getters and setters for name, organisation, and phoneNumbers

    string getName() const {
        return name;
    }

    string getOrganisation() const {
        return organisation;
    }

    vector<string> getPhoneNumbers() const {
        return phoneNumbers;
    }
};

class HashTableRecord {
private:
    int key;
    PhoneRecord* element;
    HashTableRecord* next;

public:
    HashTableRecord(int k, PhoneRecord* rec)
        : key(k), element(rec), next(nullptr) {}

    // Getters and setters for key, record, and next

    int getKey() const {
        return key;
    }

    PhoneRecord* getRecord() const {
        return element;
    }

    HashTableRecord* getNext() const {
        return next;
    }

    void setNext(HashTableRecord* nxt) {
        next = nxt;
    }
};

class PhoneBook {
private:
    static const int HASH_TABLE_SIZE = 263; // Fixed size of the hash table
    HashTableRecord* hashTable[HASH_TABLE_SIZE];
    int m = 263;

public:
    PhoneBook() {
        // Initialize the hash table slots to nullptr
        for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
            hashTable[i] = nullptr;
        }
    }

    // Function to compute the hash value for a given string
    long long computeHash(const string& str) {
        // Implement a hash function for strings
        long long p = 1000000007;
        long long x = 263;
        long long hashvalue = 0;
        long long x_pow_i = 1;
        int i;
        for(i=0;i<str.size();i++){
            long long s = str[i];
            hashvalue = (hashvalue + (s * ((x_pow_i) % p)));
            x_pow_i = (x_pow_i * x) % p;
         hashvalue = hashvalue%m; }
        // cout<<hashvalue<<" ";
        return hashvalue;
    }

    // Implement methods for adding, deleting, and fetching contacts

    void addContact(PhoneRecord* record) {
        // Implement adding a contact to the phone book
        string name = record->getName();
        vector<string> words;
        string currentWord = "";
        for (char c : name) {
            if (c == ' ') {
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
            } else {
                currentWord += c;
            }
        }
        if (!currentWord.empty()) {
            words.push_back(currentWord);
        }
        // for(int m=0;m<words.size();m++)
        // {
        //     // cout<<words[m]<<endl;
        // }

        for(int i=0;i<words.size();i++){
            long long int hashvalue = computeHash(words[i]);
            HashTableRecord* newRecord = new HashTableRecord(hashvalue,record);
            HashTableRecord*node;
             
          if(hashTable[hashvalue]==nullptr)
          {
            hashTable[hashvalue]=newRecord;
            hashTable[hashvalue]->setNext(nullptr);
            // cout<<hashvalue<<newRecord->getRecord().getName()<<endl;
          }
          else{
            node=hashTable[hashvalue];
            newRecord->setNext(node);
            hashTable[hashvalue]=newRecord;
            // cout<<hashvalue<<newRecord->getRecord().getName()<<"collision"<<endl;
          }


            // newRecord->setNext(hashTable[hashvalue]);
            // hashTable[hashvalue]=newRecord;
            // cout<<"Added ";
        }
    }

    bool deleteContact(const string* searchName) {
        // Implement deleting a contact from the phone book
        const std::string& s = *searchName;
        vector<PhoneRecord*> searches = fetchContacts(searchName);
        if(searches.empty()){
            return false;
        }
        for(int j=0;j<searches.size();j++){
        PhoneRecord* deleting = searches[j];
        // vector<HashTableRecord*> deleted;
        for(int i=0;i<HASH_TABLE_SIZE;i++){
            HashTableRecord* current = hashTable[i];
            HashTableRecord* prev = nullptr;
            while(current!=nullptr){
            if(current->getRecord()->getName() == deleting->getName()){
                // deleted.push_back(current);
                if(prev!=nullptr){
                    prev->setNext(current->getNext());
                }
                else {
                    hashTable[i]=current->getNext();
                }
                delete current;
            }
            else{
                prev = current;
            }
            current=current->getNext();
        }
    } }
    return true;
    }

    vector<PhoneRecord*> fetchContacts(const string* query) {
        // Implement fetching contacts based on the query
        // You may need to split the query into words and hash them separately
        // Then, retrieve and merge the records from the appropriate hash table slots
        // Sort and return the merged records
        vector<PhoneRecord*> result;
        vector<string> words;
        string currentWord = "";
        const std::string& query_str = *query;
        for (char c : query_str) {
            // char c=query[i];
            if (c == ' ') {
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
            } else {
                currentWord += c;
            }
        }
        if (!currentWord.empty()) {
            words.push_back(currentWord);
        }
        // for(int m=0;m<words.size();m++)
        // {
        //     cout<<words[m]<<endl;
        // }
        for(int i=0;i<words.size();i++){
            string shabd = words[i];
            long long int  key = computeHash(shabd);
            // vector<HashTableRecord*> records;
            HashTableRecord*records = hashTable[key];
            // cout<<records.size();
            while(records!=nullptr){
                PhoneRecord* phoneRec = records->getRecord();
                result.push_back(phoneRec);
                // cout<<records->getRecord().getName()<<" ";
                records=records->getNext();   
            }
        }
        return result;
    }

    void readRecordsFromFile(string file_path){
    // PhoneBook phoneBook;
    ifstream iF(file_path);
    if (!iF.is_open()) {
        cerr << "Failed" << endl;
        return;
    }
    string l;
    while (getline(iF, l)) { 
        stringstream ss(l);
        string st;
        vector<string> arr;
        while (getline(ss, st, ',')) {
            arr.push_back(st);
        }
        // string n = arr[0];
        vector<string> phoneNumbers;
        for (size_t i = 1; i < arr.size() - 1; ++i) {
            phoneNumbers.push_back(arr[i]);
        }
        string organisation = arr[arr.size() - 1];
        PhoneRecord*record=new PhoneRecord(arr[0],arr[arr.size()-1],phoneNumbers);
        addContact(record);
        // cout<<record.getName()<<endl;
    }
    iF.close();
}
};
