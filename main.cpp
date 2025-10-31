/**
 * @file main.cpp
 * @brief Programa principal del sistema de gestion de sensores IoT
 * @author Sistema de Monitoreo
 * @version 1.0
 * @date 2024
 * 
 * Este archivo contiene la implementacion del programa principal que gestiona
 * sensores IoT mediante una estructura de datos de lista enlazada simple.
 * Permite la captura de datos desde dispositivos Arduino via puerto serial,
 * asi como el registro manual de sensores y mediciones.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "SensorBase.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "ListaSensor.h"
#include "SerialPort.h"

// Definicion de tipo para lista polimorfica de sensores
typedef ListaSensor<SensorBase*> ColeccionSensores;

/**
 * @brief Captura datos del dispositivo Arduino mediante comunicacion serial
 * 
 * Esta funcion establece una conexion con un dispositivo Arduino conectado
 * via puerto USB y captura datos en tiempo real. Los datos recibidos son
 * parseados y almacenados en la coleccion de sensores.
 * 
 * @param registro Puntero a la coleccion de sensores donde se almacenaran los datos
 * 
 * @details El formato esperado de datos es: TIPO ID VALOR
 *          - TIPO: 'T' para temperatura, 'P' para presion
 *          - ID: Identificador unico del sensor
 *          - VALOR: Medicion numerica (float para temperatura, int para presion)
 * 
 * @note La funcion entra en un ciclo infinito hasta que se interrumpa con Ctrl+C
 * @warning Requiere permisos de lectura en el puerto serial en sistemas Unix
 */
void capturarDatosHardware(ColeccionSensores* registro) {
    std::cout << "\n+------------------------------------------------+" << std::endl;
    std::cout << "|      CAPTURA DE DATOS DESDE ARDUINO            |" << std::endl;
    std::cout << "+------------------------------------------------+\n" << std::endl;
    
    // Inicializar conexion serial
    SerialPort conexion;
    
    // Solicitar ruta del puerto al usuario
    std::cout << "Puertos disponibles segun sistema operativo:" << std::endl;
    std::cout << "  Linux:   /dev/ttyACM0, /dev/ttyUSB0" << std::endl;
    std::cout << "  Mac:     /dev/cu.usbmodem*, /dev/cu.usbserial*" << std::endl;
    std::cout << "  Windows: (utilizar modo de simulacion - Opcion 6)" << std::endl;
    std::cout << "\nEspecifique la ruta del puerto: ";
    
    std::string rutaPuerto;
    std::cin >> rutaPuerto;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Establecer conexion con el puerto
    if (!conexion.abrir(rutaPuerto, 9600)) {
        std::cout << "\n[ERROR] Fallo en la conexion con el dispositivo" << std::endl;
        std::cout << "\nAcciones sugeridas:" << std::endl;
        std::cout << "  1. Confirmar conexion fisica del Arduino" << std::endl;
        std::cout << "  2. En Linux, otorgar permisos: sudo chmod 666 " << rutaPuerto << std::endl;
        std::cout << "  3. Verificar puerto correcto en IDE Arduino (Menu Herramientas)" << std::endl;
        std::cout << "  4. Reconectar el dispositivo" << std::endl;
        return;
    }
    
    std::cout << "\n[OK] Conexion establecida correctamente" << std::endl;
    std::cout << "[OK] Aguardando transmision de datos..." << std::endl;
    std::cout << "[OK] Presione Ctrl+C para finalizar captura\n" << std::endl;
    std::cout << "------------------------------------------------\n" << std::endl;
    
    std::string buffer;
    int contadorLecturas = 0;
    
    // Ciclo de lectura continua
    while (true) {
        if (conexion.leerLinea(buffer)) {
            // Filtrar mensajes de sistema del Arduino
            if (buffer.find("===") != std::string::npos || 
                buffer.find("Arduino") != std::string::npos ||
                buffer.find("Formato") != std::string::npos ||
                buffer.empty()) {
                continue;
            }
            
            std::cout << "[RX] Datos recibidos: " << buffer << std::endl;
            
            // Parseo de la cadena recibida
            std::istringstream parser(buffer);
            char tipoDispositivo;
            std::string identificador;
            
            if (!(parser >> tipoDispositivo >> identificador)) {
                std::cout << "[WARN] Formato de datos incorrecto, descartando..." << std::endl;
                continue;
            }
            
            // Verificar existencia previa del sensor
            SensorBase* dispositivoExistente = nullptr;
            registro->iterar([&dispositivoExistente, &identificador](SensorBase* dispositivo) {
                if (std::string(dispositivo->getNombre()) == identificador) {
                    dispositivoExistente = dispositivo;
                }
            });
            
            if (tipoDispositivo == 'T' || tipoDispositivo == 't') {
                float medicion;
                if (!(parser >> medicion)) {
                    std::cout << "[WARN] Valor de temperatura invalido" << std::endl;
                    continue;
                }
                
                if (dispositivoExistente == nullptr) {
                    // Instanciar nuevo sensor termico
                    SensorTemperatura* nuevoDispositivo = new SensorTemperatura(identificador.c_str());
                    nuevoDispositivo->agregarLectura(medicion);
                    registro->insertarAlFinal(nuevoDispositivo);
                    std::cout << "[OK] Sensor termico '" << identificador << "' registrado" << std::endl;
                } else {
                    // Actualizar sensor existente
                    SensorTemperatura* sensorTermico = dynamic_cast<SensorTemperatura*>(dispositivoExistente);
                    if (sensorTermico) {
                        sensorTermico->agregarLectura(medicion);
                        std::cout << "[OK] Medicion almacenada en '" << identificador << "': " 
                                  << medicion << " grados C" << std::endl;
                    }
                }
                
            } else if (tipoDispositivo == 'P' || tipoDispositivo == 'p') {
                int medicion;
                if (!(parser >> medicion)) {
                    std::cout << "[WARN] Valor de presion invalido" << std::endl;
                    continue;
                }
                
                if (dispositivoExistente == nullptr) {
                    // Instanciar nuevo sensor de presion
                    SensorPresion* nuevoDispositivo = new SensorPresion(identificador.c_str());
                    nuevoDispositivo->agregarLectura(medicion);
                    registro->insertarAlFinal(nuevoDispositivo);
                    std::cout << "[OK] Sensor de presion '" << identificador << "' registrado" << std::endl;
                } else {
                    // Actualizar sensor existente
                    SensorPresion* sensorPresion = dynamic_cast<SensorPresion*>(dispositivoExistente);
                    if (sensorPresion) {
                        sensorPresion->agregarLectura(medicion);
                        std::cout << "[OK] Medicion almacenada en '" << identificador << "': " 
                                  << medicion << " Pascales" << std::endl;
                    }
                }
            } else {
                std::cout << "[WARN] Tipo de dispositivo no reconocido: " << tipoDispositivo << std::endl;
                continue;
            }
            
            contadorLecturas++;
            std::cout << "[INFO] Total de mediciones capturadas: " << contadorLecturas << "\n" << std::endl;
        }
    }
}



/**
 * @brief Despliega el menu principal de opciones del sistema
 * 
 * Muestra en pantalla las opciones disponibles para el usuario,
 * incluyendo registro de sensores, captura de mediciones,
 * procesamiento de datos y conexion con Arduino.
 * 
 * @return void
 */
void desplegarMenu() {
    std::cout << "\n||================================||" << std::endl;
    std::cout << "|| Plataforma de Monitoreo IoT    ||" << std::endl;
    std::cout << "||================================||" << std::endl;
    std::cout << "|| 1. Registrar Sensor Termico    ||" << std::endl;
    std::cout << "|| 2. Registrar Sensor de Presion ||" << std::endl;
    std::cout << "|| 3. Capturar Nueva Medicion     ||" << std::endl;
    std::cout << "|| 4. Procesar Datos Almacenados  ||" << std::endl;
    std::cout << "|| 5. Finalizar Sistema           ||" << std::endl;
    std::cout << "|| 6. Conectar Arduino (Serial)   ||" << std::endl;
    std::cout << "||================================||" << std::endl;
    std::cout << "Ingrese su seleccion: ";
}

/**
 * @brief Funcion principal del programa
 * 
 * Punto de entrada del sistema de gestion de sensores IoT.
 * Inicializa la estructura de datos principal, presenta el menu
 * de opciones y procesa las selecciones del usuario en un ciclo
 * hasta que se solicite finalizar el sistema.
 * 
 * @return int Codigo de retorno (0 indica ejecucion exitosa)
 * 
 * @details El programa permite:
 *          - Registrar sensores termicos y de presion
 *          - Capturar mediciones manualmente
 *          - Conectar con Arduino via puerto serial
 *          - Procesar datos almacenados
 *          - Liberar memoria al finalizar
 */
int main() {
    std::cout << "\n+------------------------------------------------+" << std::endl;
    std::cout << "|  PLATAFORMA DE GESTION DE SENSORES IoT        |" << std::endl;
    std::cout << "|  Sistema Polimorfico de Monitoreo             |" << std::endl;
    std::cout << "+------------------------------------------------+\n" << std::endl;
    
    // Inicializar estructura de datos principal
    ColeccionSensores* registro = new ColeccionSensores();
    
    int seleccion;
    bool sistemaActivo = true;
    
    while (sistemaActivo) {
        desplegarMenu();
        std::cin >> seleccion;
        
        switch (seleccion) {
            case 1: {
                // Registro de sensor termico
                std::string codigo;
                std::cout << "\nCodigo del dispositivo (ejemplo: TEMP-001): ";
                std::cin >> codigo;
                
                SensorTemperatura* nuevoDispositivo = new SensorTemperatura(codigo.c_str());
                registro->insertarAlFinal(nuevoDispositivo);
                std::cout << "Sensor termico 'T-" << codigo << "' incorporado al sistema" << std::endl;
                break;
            }
            
            case 2: {
                // Registro de sensor de presion
                std::string codigo;
                std::cout << "\nCodigo del dispositivo (ejemplo: PRES-105): ";
                std::cin >> codigo;
                
                SensorPresion* nuevoDispositivo = new SensorPresion(codigo.c_str());
                registro->insertarAlFinal(nuevoDispositivo);
                std::cout << "Sensor de presion 'P-" << codigo << "' incorporado al sistema" << std::endl;
                break;
            }
            
            case 3: {
                // Captura de nueva medicion
                std::string codigo;
                std::cout << "\nCodigo del sensor objetivo: ";
                std::cin >> codigo;
                
                SensorBase* dispositivoLocalizado = nullptr;
                registro->iterar([&dispositivoLocalizado, &codigo](SensorBase* dispositivo) {
                    if (std::string(dispositivo->getNombre()) == codigo) {
                        dispositivoLocalizado = dispositivo;
                    }
                });
                
                if (dispositivoLocalizado != nullptr) {
                    SensorTemperatura* sensorTermico = dynamic_cast<SensorTemperatura*>(dispositivoLocalizado);
                    SensorPresion* sensorPresion = dynamic_cast<SensorPresion*>(dispositivoLocalizado);
                    
                    if (sensorTermico) {
                        float dato;
                        std::cout << "Valor de medicion (decimal): ";
                        std::cin >> dato;
                        sensorTermico->agregarLectura(dato);
                        std::cout << "ID: " << codigo << " | Valor: " << dato << " (tipo decimal)" << std::endl;
                    } else if (sensorPresion) {
                        int dato;
                        std::cout << "Valor de medicion (entero): ";
                        std::cin >> dato;
                        sensorPresion->agregarLectura(dato);
                        std::cout << "ID: " << codigo << " | Valor: " << dato << " (tipo entero)" << std::endl;
                    }
                } else {
                    std::cout << "Dispositivo no localizado en el registro" << std::endl;
                }
                break;
            }
            
            case 4: {
                // Procesamiento de datos almacenados
                std::cout << "\n<<< Iniciando procesamiento de sensores >>>" << std::endl;
                
                registro->iterar([](SensorBase* dispositivo) {
                    std::cout << "\n>> Analizando dispositivo " << dispositivo->getNombre() << "..." << std::endl;
                    
                    // Identificacion del tipo de sensor
                    SensorTemperatura* sensorTermico = dynamic_cast<SensorTemperatura*>(dispositivo);
                    SensorPresion* sensorPresion = dynamic_cast<SensorPresion*>(dispositivo);
                    
                    if (sensorTermico) {
                        std::cout << "[Sensor Termico] Calculo de minima ejecutado" << std::endl;
                    } else if (sensorPresion) {
                        std::cout << "[Sensor Presion] Calculo de promedio ejecutado" << std::endl;
                    }
                    
                    // Invocacion polimorfica del metodo
                    dispositivo->procesarLectura();
                });
                
                std::cout << "\n<<< Procesamiento finalizado >>>" << std::endl;
                break;
            }
            
            case 5: {
                // Finalizacion del sistema y liberacion de memoria
                std::cout << "\n<<< Proceso de cierre iniciado >>>" << std::endl;
                std::cout << "[Sistema] Liberando recursos de memoria..." << std::endl;
                
                // Liberar cada dispositivo del registro
                registro->iterar([](SensorBase* dispositivo) {
                    delete dispositivo;
                });
                
                // Limpiar estructura principal
                delete registro;
                
                std::cout << "Proceso terminado. Memoria liberada correctamente." << std::endl;
                sistemaActivo = false;
                break;
            }
            
            case 6: {
                // Conexion con Arduino real
                capturarDatosHardware(registro);
                break;
            }
            
            default:
                std::cout << "Seleccion no valida. Intente nuevamente." << std::endl;
                break;
        }
    }
    
    std::cout << "\nGracias por utilizar la Plataforma de Monitoreo IoT" << std::endl;
    return 0;
}
