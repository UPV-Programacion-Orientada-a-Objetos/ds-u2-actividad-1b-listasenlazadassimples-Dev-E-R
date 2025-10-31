/**
 * @file SensorTemperatura.h
 * @brief Implementacion de sensor termico para mediciones de temperatura
 * @author Sistema de Monitoreo
 * @version 1.0
 * @date 2024
 */

#ifndef SENSORTEMPERATURA_H
#define SENSORTEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <iostream>
#include <iomanip>

/**
 * @class SensorTemperatura
 * @brief Clase especializada para dispositivos termicos
 * 
 * Hereda de SensorBase e implementa la funcionalidad especifica
 * para sensores de temperatura. Almacena mediciones de tipo float
 * y calcula el valor minimo registrado.
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float>* registroMediciones;  ///< Coleccion de mediciones termicas
    
public:
    /**
     * @brief Constructor parametrizado
     * @param identificador Codigo unico del sensor (por defecto "TERM-000")
     * 
     * Inicializa el sensor termico y crea una lista para almacenar mediciones.
     */
    SensorTemperatura(const char* identificador = "TERM-000") : SensorBase(identificador) {
        registroMediciones = new ListaSensor<float>();
        std::cout << "[Dispositivo Termico] Inicializado: " << nombre << std::endl;
    }
    
    /**
     * @brief Destructor especializado
     * 
     * Libera la memoria ocupada por el historial de mediciones.
     */
    ~SensorTemperatura() override {
        std::cout << "[Finalizacion " << nombre << "]" << std::endl;
        delete registroMediciones;
    }
    
    /**
     * @brief Incorpora una nueva medicion al registro
     * @param medida Valor de temperatura en grados Celsius
     * 
     * Agrega una nueva lectura de temperatura a la lista de mediciones.
     */
    void agregarLectura(float medida) {
        registroMediciones->insertarAlFinal(medida);
        std::cout << "[Dato] Valor decimal " << std::fixed << std::setprecision(1) 
                  << medida << " almacenado" << std::endl;
    }
    
    /**
     * @brief Implementacion del metodo abstracto de procesamiento
     * 
     * Calcula y muestra el valor minimo de temperatura registrado.
     * Si no hay mediciones disponibles, informa al usuario.
     */
    void procesarLectura() override {
        if (registroMediciones->estaVacia()) {
            std::cout << "[Dispositivo Termico] Registro vacio, sin datos para analizar" << std::endl;
            return;
        }
        
        // Determinar valor inferior del conjunto
        float valorMinimo = 999999.0f;
        registroMediciones->iterar([&valorMinimo](float medida) {
            if (medida < valorMinimo) {
                valorMinimo = medida;
            }
        });
        
        std::cout << "[Dispositivo Termico] Valor minimo detectado: " 
                  << std::fixed << std::setprecision(1) << valorMinimo << std::endl;
    }
    
    /**
     * @brief Implementacion del metodo abstracto de visualizacion
     * 
     * Muestra informacion detallada del sensor incluyendo su tipo,
     * identificador, cantidad de mediciones y el historial completo
     * de temperaturas registradas.
     */
    void imprimirInfo() const override {
        std::cout << "\n>>> Detalles del Dispositivo <<<" << std::endl;
        std::cout << "Categoria: Sensor Termico" << std::endl;
        std::cout << "Identificador: " << nombre << std::endl;
        std::cout << "Mediciones registradas: " << registroMediciones->getTamanio() << std::endl;
        
        if (!registroMediciones->estaVacia()) {
            std::cout << "Conjunto de datos: ";
            registroMediciones->iterar([](float medida) {
                std::cout << std::fixed << std::setprecision(1) << medida << " grados ";
            });
            std::cout << std::endl;
        }
        std::cout << "================================\n" << std::endl;
    }
    
    /**
     * @brief Accede al registro de mediciones
     * @return Puntero a la lista de mediciones termicas
     * 
     * Permite acceso directo al historial para operaciones avanzadas.
     */
    ListaSensor<float>* getHistorial() {
        return registroMediciones;
    }
};

#endif // SENSORTEMPERATURA_H
