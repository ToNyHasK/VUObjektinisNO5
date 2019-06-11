//
// Created by Tomas on 6/5/2019.
//
#include "header.h"

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool isLink(const std::string& link) {
    const std::regex http
            ("https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");
    const std::regex www
            ("[-a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)");
    return (std::regex_match(link, http) || std::regex_match(link, www));
}
void removePunc (std::string& word) {
    if (isLetter(word[word.length()-1])) {
        return;
    }
    switch (word[word.length()-1]) {
        case '/': return;
        default: word.erase(word.length()-1); return;
    }
}
void checkStream(const std::ifstream& file) {
    // Check if you can open the file
    if (!file.is_open()) {
        std::cout << "Error while opening a file" << std::endl;
        exit(EXIT_FAILURE);
    }
}
void processText(std::string& text, mapType& words, setType& url) {
    std::set<std::string> links;
    int rowCounter = 1;
    int wordStart = 0;
    int lengthOfWord = 0;
    std::ifstream file("../source.txt");
    checkStream(file);
    while (std::getline(file, text)) {
        for (int i = 0; i < text.length(); i++) {
            if (text[i] != '\n' && text[i] != ' ') {
                if (text[i] >= 'A' && text[i] <= 'Z') {
                    text[i] += 32;
                }
                if (text[i-1] == '\n'  || text[i-1] == ' ' || i == 0) {
                    wordStart = i;
                }
                lengthOfWord++;
                if (text[i+1] == ' ' || text[i+1] == '\n' || i+1 == text.length()) {
                    std::string theWord = text.substr(wordStart, lengthOfWord);
                        if (isLink(theWord)) {
                            url.insert(theWord);
                        }
                            removePunc(theWord);
                            auto finder = words.find(theWord);
                            words[theWord].first.insert(rowCounter);
                            if (finder != words.end()) {
                                words[theWord].second++;
                            } else {
                                words[theWord].second = 1;
                            }
                            lengthOfWord = 0;
                }
                continue;
            } else if (text[i] == ' ') {
                continue;
            }
        }
        rowCounter++;
    }
    file.close();
}
void printResults(std::string text, mapType words, setType url) {
    // Printing results
    std::ofstream myfile;
    myfile.open("cross-ref.txt");
    processText(text, words, url);
    int m = 0; // max words
    // ieskom max words
    for (auto& max : words) {
        if (m < max.second.second ) {
            m = max.second.second;
        }
    }
    int n = 1;
    for (int i = 2; i <= m; i++) {
        for (auto &elem : words) {
            if (elem.second.second == i) {
                myfile << elem.first << " : "<< elem.second.second << " | ";
                int eil = 0;
                for (auto it = elem.second.first.begin(); it != elem.second.first.end(); ++it) {
                    myfile << (*it) << " ";
                    eil++;
                }
                if (n%2 != 0) {
                    myfile << std::right << std::setw(35-eil);
                }
                if (n%2 == 0)
                    myfile << std::endl;

                    n++;
            }
        }
    }
    myfile << "URL address: " << "\n";
    for (std::set<std::string>::iterator it = url.begin(); it != url.end(); ++it) {
        myfile << (*it) << std::endl;
    }
    myfile.close();
}

