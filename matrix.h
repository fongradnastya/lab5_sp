/*! \file    matrix.h
 *  \brief   Функции для работы с матрицами
 *
 *  \details Заголовочный файл, содержащий функции для просмотра 
 *           и модификации двумерных массивов
 */

/*!
 * \brief Выводит элементв матрицы в консоль
 * \param[in] matrix матрица для вывода
 * \param[in] sizeX число столбцов в матрице
 * \param[in] sizeY число строк в матрице
 */
void PrintMatrix(int** matrix, int sizeX, int sizeY);

/*!
 * \brief Заменяет нулями строку с наименьшим элементом
 * \param[in] matrix исходная матрица
 * \param[in] size размер матрицы
 * \return номер изменённой строки
 */
int replaceMinString(int** matrix, int size);

/*!
 * \brief Удаляет столбец с наибольшим элементом
 * \param[in] matrix исходная матрица
 * \param[out] newMatrix матрица без одного из столбцов
 * \param[in] size размер исходной матрицы
 * \return номер удалённого столбца
 */
int deleteMaxColumn(int** matrix, int** newMatrix, int size);

/*!
 * \brief Производит манипуляции над переданной маторицей
 * \param[in] matrix исходная матрица
 * \param[in] size размер исходной матрицы
 */
void matrixProcessing(int** matrix, int size);

/*!
 * \brief Считывает матрицу из переданной строки
 * \param[in] str строка из разделённых пробелом элементов
 * \return квадратная матрица из элементов строки
 */
int** getMatrixFromString(char* str);