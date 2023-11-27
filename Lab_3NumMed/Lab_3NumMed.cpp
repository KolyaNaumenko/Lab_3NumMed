#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

std::unordered_map<std::string, std::vector<std::string>> ReadGraphFromFile(const std::string& filePath) {
    std::unordered_map<std::string, std::vector<std::string>> graph;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string page;
        std::getline(ss, page, ':');

        if (!ss.eof()) {
            std::string links;
            std::getline(ss, links);
            std::istringstream linksStream(links);
            std::vector<std::string> linksVector;

            while (std::getline(linksStream, links, ',')) {
                linksVector.push_back(links);
            }

            graph[page] = linksVector;
        }
        else {
            graph[page] = std::vector<std::string>();
        }
    }

    return graph;
}

void WritePageRank(const std::string& filePath, const std::unordered_map<std::string, double>& pageRank) {
    std::ofstream writer(filePath, std::ios::out | std::ios::trunc); // Open file for writing, truncate if exists, create if not

    if (writer.is_open()) {
        std::cout << "PageRank результати:" << std::endl;
        for (const auto& kvp : pageRank) {
            std::cout << kvp.first << ": " << kvp.second << std::endl;
            writer << kvp.first << ": " << kvp.second << std::endl;
        }
        writer.close();
    }
    else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }
}

int main() {
    // Reading graph from file
    std::unordered_map<std::string, std::vector<std::string>> graph = ReadGraphFromFile("input.txt");

    // Initialize PageRank for every page
    std::unordered_map<std::string, double> pageRank;
    for (const auto& page : graph) {
        pageRank[page.first] = 1.0;
    }

    int iterationsLimit = 30;
    double dampingFactor = 0.85;

    for (int i = 0; i < iterationsLimit; ++i) {
        std::unordered_map<std::string, double> newPageRank;

        for (const auto& page : graph) {
            double rank = (1 - dampingFactor);

            for (const auto& referringPage : graph) {
                if (std::find(referringPage.second.begin(), referringPage.second.end(), page.first) != referringPage.second.end()) {
                    rank += dampingFactor * (pageRank[referringPage.first] / referringPage.second.size());
                }
            }

            newPageRank[page.first] = rank;
        }

        pageRank = newPageRank;
    }

    WritePageRank("output.txt", pageRank);

    return 0;
}