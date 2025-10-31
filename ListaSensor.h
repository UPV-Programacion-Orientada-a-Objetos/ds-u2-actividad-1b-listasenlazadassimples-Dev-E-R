/**
 * @file ListaSensor.h
 * @brief Implementacion de lista enlazada simple generica
 * @author Sistema de Monitoreo
 * @version 1.0
 * @date 2024
 */

#ifndef LISTASENSOR_H
#define LISTASENSOR_H

#include "Nodo.h"
#include <iostream>

/**
 * @class ListaSensor
 * @brief Estructura de datos enlazada generica para almacenamiento dinamico
 * 
 * Implementa una lista enlazada simple que puede almacenar cualquier tipo de dato.
 * Proporciona operaciones fundamentales de insercion, busqueda, iteracion y liberacion.
 * Cumple con la Regla de los Tres para gestion correcta de memoria dinamica.
 * 
 * @tparam T Tipo de dato que almacenara la lista
 */
template <typename T>
class ListaSensor {
private:
    Nodo<T>* primero;  ///< Referencia al elemento inicial de la lista
    int elementos;     ///< Contador de elementos presentes en la lista
    
public:
    /**
     * @brief Constructor predeterminado
     * 
     * Inicializa una lista vacia con puntero nulo y contador en cero.
     */
    ListaSensor() : primero(nullptr), elementos(0) {
        std::cout << "[Inicializacion] Estructura de lista creada" << std::endl;
    }
    
    /**
     * @brief Constructor de copia
     * @param origen Lista a copiar
     * 
     * Crea una copia profunda de otra lista, duplicando todos sus elementos.
     */
    ListaSensor(const ListaSensor& origen) : primero(nullptr), elementos(0) {
        std::cout << "[Duplicacion] Proceso de copia iniciado" << std::endl;
        duplicarDesde(origen);
    }
    
    /**
     * @brief Operador de asignacion por copia
     * @param origen Lista a copiar
     * @return Referencia a esta lista
     * 
     * Asigna el contenido de otra lista a esta, liberando primero
     * cualquier contenido previo.
     */
    ListaSensor& operator=(const ListaSensor& origen) {
        std::cout << "[Asignacion] Operador de copia invocado" << std::endl;
        if (this != &origen) {
            vaciar();
            duplicarDesde(origen);
        }
        return *this;
    }
    
    /**
     * @brief Destructor
     * 
     * Libera toda la memoria dinamica ocupada por los nodos de la lista.
     */
    ~ListaSensor() {
        std::cout << "[Destruccion] Liberando estructura de lista..." << std::endl;
        vaciar();
    }
    
    /**
     * @brief Inserta un elemento al final de la lista
     * @param contenido Dato a insertar
     * 
     * Crea un nuevo nodo con el contenido proporcionado y lo agrega
     * al final de la lista enlazada.
     */
    void insertarAlFinal(T contenido) {
        Nodo<T>* nuevoElemento = new Nodo<T>(contenido);
        
        if (primero == nullptr) {
            primero = nuevoElemento;
        } else {
            Nodo<T>* navegador = primero;
            while (navegador->siguiente != nullptr) {
                navegador = navegador->siguiente;
            }
            navegador->siguiente = nuevoElemento;
        }
        elementos++;
        std::cout << "[Agregacion] Elemento de tipo<" << typeid(T).name() << "> agregado" << std::endl;
    }
    
    /**
     * @brief Busca un elemento en la lista
     * @param contenido Dato a buscar
     * @return Puntero al nodo que contiene el dato, nullptr si no se encuentra
     * 
     * Recorre la lista secuencialmente comparando cada elemento
     * con el dato buscado.
     */
    Nodo<T>* buscar(T contenido) const {
        Nodo<T>* navegador = primero;
        while (navegador != nullptr) {
            if (navegador->dato == contenido) {
                return navegador;
            }
            navegador = navegador->siguiente;
        }
        return nullptr;
    }
    
    /**
     * @brief Obtiene la cantidad de elementos en la lista
     * @return Numero de elementos almacenados
     */
    int getTamanio() const {
        return elementos;
    }
    
    /**
     * @brief Verifica si la lista esta vacia
     * @return true si la lista no contiene elementos, false en caso contrario
     */
    bool estaVacia() const {
        return primero == nullptr;
    }
    
    /**
     * @brief Obtiene el primer nodo de la lista
     * @return Puntero al primer nodo, nullptr si la lista esta vacia
     */
    Nodo<T>* getCabeza() const {
        return primero;
    }
    
    /**
     * @brief Itera sobre todos los elementos aplicando una operacion
     * @tparam Operacion Tipo de la funcion a aplicar
     * @param operacion Funcion que se aplicara a cada elemento
     * 
     * Recorre la lista completa aplicando la operacion proporcionada
     * a cada elemento. Util para operaciones como impresion o calculo.
     */
    template <typename Operacion>
    void iterar(Operacion operacion) const {
        Nodo<T>* navegador = primero;
        while (navegador != nullptr) {
            operacion(navegador->dato);
            navegador = navegador->siguiente;
        }
    }
    
    /**
     * @brief Elimina todos los elementos de la lista
     * 
     * Libera la memoria de todos los nodos y reinicia el contador.
     * Despues de esta operacion, la lista queda vacia.
     */
    void vaciar() {
        while (primero != nullptr) {
            Nodo<T>* temporal = primero;
            primero = primero->siguiente;
            std::cout << "[Liberacion] Elemento<" << typeid(T).name() << "> eliminado" << std::endl;
            delete temporal;
            elementos--;
        }
    }
    
private:
    /**
     * @brief Metodo auxiliar para duplicar contenido de otra lista
     * @param origen Lista fuente de la copia
     * 
     * Copia todos los elementos de la lista origen a esta lista.
     * Utilizado por el constructor de copia y operador de asignacion.
     */
    void duplicarDesde(const ListaSensor& origen) {
        if (origen.primero == nullptr) {
            return;
        }
        
        Nodo<T>* navegadorOrigen = origen.primero;
        while (navegadorOrigen != nullptr) {
            insertarAlFinal(navegadorOrigen->dato);
            navegadorOrigen = navegadorOrigen->siguiente;
        }
    }
};

#endif // LISTASENSOR_H
