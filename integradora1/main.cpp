
 /*
 * SonarQube, open source software quality management tool.
 * Copyright (C) 2008-2013 SonarSource
 * http://github.com/SonarOpenCommunity/sonar-cxx
 *
 * SonarQube is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * SonarQube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>

// Función para leer el contenido de un archivo
std::string readFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (file.is_open()) {
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    } else {
        std::cerr << "Error al abrir el archivo: " << fileName << std::endl;
        return "";
    }
}

// Función para calcular el arreglo Z
std::vector<size_t> calculateZ(const std::string& s) {
    size_t n = s.size();
    std::vector<size_t> z(n, 0);
    size_t l = 0, r = 0;
    for (size_t i = 1; i < n; ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

// Función para encontrar si un string contiene otro string y devuelve la posición
bool contains(const std::string& haystack, const std::string& needle, size_t& position) {
    std::string combined = needle + "$" + haystack; // Concatenar patrón, separador único y texto
    std::vector<size_t> z = calculateZ(combined);

    for (size_t i = needle.size() + 1; i < z.size(); ++i) {
        if (z[i] >= needle.size()) {
            position = i - needle.size() - 1;
            return true;
        }
    }
    return false;
}


// Función para encontrar el palíndromo más largo en un string
std::pair<size_t, size_t> longestPalindrome(const std::string& s, std::string& palindrome) {
    size_t n = s.size();
    if (n == 0) return {0, 0};

    size_t start = 0, maxLength = 1;
    std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));

    for (size_t i = 0; i < n; ++i) dp[i][i] = true;

    for (size_t len = 2; len <= n; ++len) {
        for (size_t i = 0; i <= n - len; ++i) {
            size_t j = i + len - 1;
            if (s[i] == s[j] && (len == 2 || dp[i + 1][j - 1])) {
                dp[i][j] = true;
                if (len > maxLength) {
                    start = i;
                    maxLength = len;
                }
            }
        }
    }

    palindrome = s.substr(start, maxLength);

    // Eliminar caracteres no visibles
    palindrome.erase(std::remove_if(palindrome.begin(), palindrome.end(), [](unsigned char c) {
        return std::isspace(c); 
    }), palindrome.end());

    return {start + 1, start + maxLength};
}

// Función para encontrar el substring común más largo entre dos strings
std::pair<size_t, size_t> longestCommonSubstring(const std::string& s1, const std::string& s2, std::string& substring) {
    size_t m = s1.size(), n = s2.size();
    size_t maxLength = 0, endPos = 0;
    std::vector<std::vector<size_t>> dp(m + 1, std::vector<size_t>(n + 1, 0));

    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endPos = i - 1;
                }
            }
        }
    }
    substring = s1.substr(endPos - maxLength + 1, maxLength);
    return {endPos - maxLength + 2, endPos + 1};
}

int main() {
    // Nombres de los archivos
    const std::string transmissionFile1 = "transmission1.txt";
    const std::string transmissionFile2 = "transmission2.txt";
    const std::string mcodeFile1 = "mcode1.txt";
    const std::string mcodeFile2 = "mcode2.txt";
    const std::string mcodeFile3 = "mcode3.txt";

    // Variables para almacenar el contenido de cada archivo
    std::string transmissionData1 = readFile(transmissionFile1);
    std::string transmissionData2 = readFile(transmissionFile2);
    std::string mcodeData1 = readFile(mcodeFile1);
    std::string mcodeData2 = readFile(mcodeFile2);
    std::string mcodeData3 = readFile(mcodeFile3);

    // Parte 1: Verificar si el contenido de los archivos mcode está en los archivos de transmisión
    size_t position;
    std::vector<std::string> transmissions = {transmissionData1, transmissionData2};
    std::vector<std::string> mcodeFiles = {mcodeData1, mcodeData2, mcodeData3};

    std::cout << "Parte 1:" << std::endl;
    for (size_t i = 0; i < transmissions.size(); ++i) {
        for (size_t j = 0; j < mcodeFiles.size(); ++j) {
            if (contains(transmissions[i], mcodeFiles[j], position)) {
                std::cout << "true " << position + 1 << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
        }
    }

    // Parte 2: Encontrar el palíndromo más largo en los archivos de transmisión
    std::cout << "Parte 2:" << std::endl;
    for (size_t i = 0; i < transmissions.size(); ++i) {
        std::string palindrome;
        std::pair<size_t, size_t> result = longestPalindrome(transmissions[i], palindrome);
        std::cout << result.first << " " << result.second << " " << palindrome << std::endl;
    }

    // Parte 3: Encontrar el substring común más largo entre los archivos de transmisión
    std::cout << "Parte 3:" << std::endl;
    std::string commonSubstring;
    std::pair<size_t, size_t> commonSubstringResult = longestCommonSubstring(transmissionData1, transmissionData2, commonSubstring);
    std::cout << commonSubstringResult.first << " " << commonSubstringResult.second << " " << commonSubstring << std::endl;

    return 0;
}

