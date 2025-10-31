/**
 * @file Nodo.h
 * @brief Estructura de nodo generico para lista enlazada
 * @author Sistema de Monitoreo
 * @version 1.0
 * @date 2024
 */

#ifndef NODO_H
#define NODO_H

/**
 * @struct Nodo
 * @brief Estructura fundamental de elemento enlazado generico
 * 
 * Representa un nodo individual en una lista enlazada simple.
 * Utiliza plantillas (templates) para lograr independencia del tipo de dato.
 * 
 * @tparam T Tipo de dato que almacenara el nodo
 */
template <typename T>
struct Nodo {
    T dato;              ///< Contenido almacenado en el elemento
    Nodo<T>* siguiente;  ///< Referencia al proximo elemento en la secuencia
    
    /**
     * @brief Constructor con inicializacion de datos
     * @param contenido Valor a almacenar en el nodo
     * 
     * Crea un nuevo nodo con el contenido especificado y puntero siguiente nulo.
     */
    Nodo(T contenido) : dato(contenido), siguiente(nullptr) {}
};

#endif // NODO_H
