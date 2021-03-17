#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include <omp.h>

void printMatrix(const std::vector<std::vector<double>> &vector);
void initMatrix(std::vector<std::vector<double>> &matrix);
std::vector<std::vector<double>> mul(const std::vector<std::vector<double>> &firstVector,
                                     const std::vector<std::vector<double>> &secondVector);

int main()
{
    size_t rowFirst = 0;
    size_t columnFirst = 0;

    size_t rowSecond = 0;
    size_t columnSecond = 0;

    std::cout << "Enter the size of first matrix: " << std::flush;
    std::cin >> rowFirst >> columnFirst;

    std::cout << "Enter the size of second matrix: " << std::flush;
    std::cin >> rowSecond >> columnSecond;

    std::vector<std::vector<double>> firstMatrix(rowFirst, std::vector<double>(columnFirst));
    std::vector<std::vector<double>> secondMatrix(rowSecond, std::vector<double>(columnSecond));

    initMatrix(firstMatrix);
    initMatrix(secondMatrix);

//    printMatrix(firstMatrix);
//    printMatrix(secondMatrix);

    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    const auto res = mul(firstMatrix, secondMatrix);
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

//    printMatrix(res);

    std::cout << "The time is equal: " <<
                 std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() <<
                 "ms" << std::endl;

    return 0;
}

void printMatrix(const std::vector<std::vector<double>> &vector)
{
    for (size_t i = 0; i < vector.size(); ++i) {
        for (size_t j = 0; j < vector[i].size(); ++j)
            std::cout << vector[i][j] << " " << std::flush;

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void initMatrix(std::vector<std::vector<double>> &matrix)
{
    std::default_random_engine dre(0);
    std::uniform_real_distribution<> uid(-228, 322);

    for (size_t i = 0; i < matrix.size(); ++i)
        for (size_t j = 0; j < matrix[i].size(); ++j)
            matrix[i][j] = uid(dre);
}

std::vector<std::vector<double>> mul(const std::vector<std::vector<double>> &firstVector,
                                     const std::vector<std::vector<double>> &secondVector)
{
    const size_t n = secondVector.size();
    const size_t row = firstVector.size();
    const size_t column = secondVector.at(0).size();

    std::vector<std::vector<double>> result(row, std::vector<double>(column, 0.));

    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

#   pragma omp parallel shared(firstVector, secondVector, result) num_threads(4)
    {
#       pragma omp for private(i, j, k)
        for (i = 0; i < row; ++i)
            for (j = 0; j < column; ++j)
                for (k = 0; k < n; ++k)
                    result[i][j] += firstVector[i][k] * secondVector[k][j];
    }

    return result;
}
