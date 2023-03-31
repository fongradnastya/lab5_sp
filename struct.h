/*!
 * \file struct.h
 * \brief Заголовочный файл с описанием структуры
 *
 * Данный файл содержит в себе определение основных
 * структур, используемых в программе.
*/

#ifndef WORK3_STRUCT_H
#define WORK3_STRUCT_H

/*!s
 * \brief Страна
 */
typedef struct{
    char name[100]; /*!< Название страны */
    long population; /*!< Население страны */
    long area; /*!< Общая площадь страны */
}Country;

#endif
