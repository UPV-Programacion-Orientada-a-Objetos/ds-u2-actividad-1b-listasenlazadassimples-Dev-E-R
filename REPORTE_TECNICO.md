# Sistema de Gestión de Sensores IoT
## Reporte Técnico

**Versión:** 1.0  
**Fecha:** Octubre 2024  
**Autor:** Sistema de Monitoreo

---

## Índice

1. [Introducción](#introducción)
2. [Manual Técnico](#manual-técnico)
   - [Diseño del Sistema](#diseño-del-sistema)
   - [Desarrollo](#desarrollo)
   - [Componentes](#componentes)

---

## Introducción

### Descripción General

El Sistema de Gestión de Sensores IoT es una aplicación desarrollada en C++ que permite la monitorización y gestión de dispositivos de medición en tiempo real. El sistema está diseñado con arquitectura orientada a objetos, implementando patrones de diseño que garantizan escalabilidad, mantenibilidad y eficiencia.

### Objetivos del Sistema

- **Gestión Polimórfica:** Manejar múltiples tipos de sensores a través de una interfaz común
- **Captura en Tiempo Real:** Obtener datos directamente desde dispositivos Arduino via puerto serial
- **Almacenamiento Eficiente:** Utilizar estructuras de datos dinámicas para el manejo óptimo de memoria
- **Procesamiento de Datos:** Realizar cálculos estadísticos sobre las mediciones capturadas
- **Portabilidad:** Funcionar en sistemas Linux y macOS

### Características Principales

1. **Polimorfismo:** Sistema basado en herencia que permite tratar distintos tipos de sensores de manera uniforme
2. **Gestión Dinámica de Memoria:** Implementación de la Regla de los Tres para manejo seguro de recursos
3. **Comunicación Serial:** Interfaz con dispositivos externos mediante protocolos de comunicación estándar
4. **Procesamiento Estadístico:** Cálculo automático de métricas relevantes (mínimos, promedios)
5. **Interfaz de Usuario Intuitiva:** Menú interactivo en consola para operación sencilla

---

## Manual Técnico

### Diseño del Sistema

#### Arquitectura General

El sistema sigue una arquitectura de tres capas:

```
┌─────────────────────────────────────┐
│     Capa de Presentación            │
│   (Interfaz de Usuario - main.cpp)  │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│      Capa de Lógica de Negocio      │
│  (Clases de Sensores y Gestión)     │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│    Capa de Acceso a Datos           │
│  (Lista Enlazada, SerialPort)       │
└─────────────────────────────────────┘
```

#### Patrones de Diseño Implementados

**1. Patrón Template Method**
- Clase base `SensorBase` define la interfaz
- Clases derivadas implementan comportamientos específicos

**2. Patrón Strategy**
- Diferentes algoritmos de procesamiento según el tipo de sensor
- `SensorTemperatura`: cálculo de mínimo
- `SensorPresion`: cálculo de promedio

**3. Patrón Iterator**
- Método `iterar()` en `ListaSensor` permite recorrer elementos sin exponer la estructura interna

#### Diagrama de Clases

```
                    ┌──────────────┐
                    │  SensorBase  │
                    │  (abstract)  │
                    ├──────────────┤
                    │ - nombre[50] │
                    ├──────────────┤
                    │ + getNombre()│
                    │ + procesarLectura()│
                    │ + imprimirInfo()   │
                    └──────┬───────┘
                           │
              ┌────────────┴────────────┐
              │                         │
    ┌─────────▼─────────┐    ┌─────────▼─────────┐
    │ SensorTemperatura │    │   SensorPresion    │
    ├───────────────────┤    ├────────────────────┤
    │ - registroMediciones│   │ - registroMediciones│
    │   ListaSensor<float>│   │   ListaSensor<int> │
    ├───────────────────┤    ├────────────────────┤
    │ + agregarLectura()│    │ + agregarLectura() │
    │ + procesarLectura()│   │ + procesarLectura()│
    │ + getHistorial()  │    │ + getHistorial()   │
    └───────────────────┘    └────────────────────┘
```

#### Estructura de Datos

**Lista Enlazada Simple Genérica**

```
┌──────┐    ┌──────┐    ┌──────┐    ┌──────┐
│ Nodo │───→│ Nodo │───→│ Nodo │───→│ NULL │
├──────┤    ├──────┤    ├──────┤    └──────┘
│ dato │    │ dato │    │ dato │
└──────┘    └──────┘    └──────┘
```

**Ventajas de la Implementación:**
- Tamaño dinámico (crecimiento según demanda)
- Inserción O(1) al final (con puntero al último)
- Memoria distribuida (no requiere bloque continuo)
- Eficiencia en iteraciones secuenciales

### Desarrollo

#### Tecnologías Utilizadas

| Tecnología | Versión | Propósito |
|------------|---------|-----------|
| C++ | 11 | Lenguaje de programación principal |
| CMake | 3.10+ | Sistema de construcción |
| Doxygen | 1.9+ | Generación de documentación |
| POSIX Termios | - | Comunicación serial (Linux/Mac) |

#### Metodología de Desarrollo

1. **Análisis de Requisitos**
   - Identificación de necesidades de monitoreo IoT
   - Definición de tipos de sensores a soportar
   - Especificación de operaciones requeridas

2. **Diseño**
   - Modelado de clases y relaciones
   - Definición de interfaces abstractas
   - Selección de estructuras de datos

3. **Implementación**
   - Codificación modular por componentes
   - Implementación de clases base y derivadas
   - Desarrollo de sistema de comunicación serial

4. **Pruebas**
   - Pruebas unitarias por clase
   - Pruebas de integración
   - Validación con hardware real (Arduino)

5. **Documentación**
   - Comentarios Doxygen en código fuente
   - Generación de documentación HTML
   - Elaboración de manual técnico

#### Proceso de Compilación

```bash
# Crear directorio de construcción
mkdir build && cd build

# Configurar con CMake
cmake ..

# Compilar
make

# Ejecutar
./program
```

### Componentes

#### 1. SensorBase (SensorBase.h)

**Responsabilidad:** Clase abstracta que define la interfaz común para todos los sensores.

**Atributos:**
- `char nombre[50]`: Identificador único del sensor

**Métodos:**
- `virtual void procesarLectura() = 0`: Procesa las mediciones almacenadas
- `virtual void imprimirInfo() const = 0`: Muestra información del sensor
- `const char* getNombre() const`: Retorna el identificador

**Características:**
- Destructor virtual para polimorfismo seguro
- Interfaz mínima para máxima flexibilidad

#### 2. SensorTemperatura (SensorTemperatura.h)

**Responsabilidad:** Gestión de sensores térmicos con mediciones de temperatura.

**Atributos:**
- `ListaSensor<float>* registroMediciones`: Historial de temperaturas

**Métodos Principales:**
- `void agregarLectura(float medida)`: Registra nueva temperatura
- `void procesarLectura()`: Calcula temperatura mínima
- `ListaSensor<float>* getHistorial()`: Acceso al historial

**Algoritmo de Procesamiento:**
```cpp
// Encuentra el valor mínimo de temperatura
float minimo = 999999.0f;
registroMediciones->iterar([&minimo](float valor) {
    if (valor < minimo) {
        minimo = valor;
    }
});
```

**Casos de Uso:**
- Monitoreo ambiental
- Control de temperatura en procesos industriales
- Sistemas de climatización

#### 3. SensorPresion (SensorPresion.h)

**Responsabilidad:** Gestión de sensores barométricos con mediciones de presión.

**Atributos:**
- `ListaSensor<int>* registroMediciones`: Historial de presiones

**Métodos Principales:**
- `void agregarLectura(int medida)`: Registra nueva presión
- `void procesarLectura()`: Calcula presión promedio
- `ListaSensor<int>* getHistorial()`: Acceso al historial

**Algoritmo de Procesamiento:**
```cpp
// Calcula el promedio de presiones
int suma = 0;
int contador = 0;
registroMediciones->iterar([&suma, &contador](int valor) {
    suma += valor;
    contador++;
});
double promedio = static_cast<double>(suma) / contador;
```

**Casos de Uso:**
- Monitoreo meteorológico
- Sistemas neumáticos
- Control de altitud

#### 4. ListaSensor (ListaSensor.h)

**Responsabilidad:** Estructura de datos genérica para almacenamiento dinámico.

**Atributos:**
- `Nodo<T>* primero`: Puntero al primer nodo
- `int elementos`: Contador de elementos

**Métodos Principales:**
- `void insertarAlFinal(T contenido)`: Agrega elemento
- `Nodo<T>* buscar(T contenido)`: Localiza elemento
- `void iterar(Operacion op)`: Recorre aplicando función
- `void vaciar()`: Libera toda la memoria
- `int getTamanio()`: Retorna cantidad de elementos
- `bool estaVacia()`: Verifica si está vacía

**Complejidad Algorítmica:**

| Operación | Complejidad |
|-----------|-------------|
| insertarAlFinal | O(n) |
| buscar | O(n) |
| vaciar | O(n) |
| getTamanio | O(1) |
| estaVacia | O(1) |

**Gestión de Memoria:**
- Constructor de copia para duplicación profunda
- Operador de asignación con liberación previa
- Destructor que libera todos los nodos

#### 5. Nodo (Nodo.h)

**Responsabilidad:** Elemento individual de la lista enlazada.

**Atributos:**
- `T dato`: Contenido del nodo
- `Nodo<T>* siguiente`: Referencia al siguiente nodo

**Características:**
- Template genérico para cualquier tipo
- Constructor simple con inicialización

#### 6. SerialPort (SerialPort.h)

**Responsabilidad:** Comunicación con dispositivos Arduino via puerto serial.

**Atributos:**
- `int descriptorArchivo`: File descriptor del puerto
- `bool estadoConexion`: Estado actual de conexión

**Métodos Principales:**
- `bool abrir(string rutaPuerto, int velocidad)`: Establece conexión
- `bool leerLinea(string& secuencia)`: Lee datos
- `void cerrar()`: Finaliza conexión
- `bool estaAbierto()`: Consulta estado

**Configuración del Puerto:**
- Velocidades soportadas: 9600, 19200, 38400, 57600, 115200 bps
- Formato: 8N1 (8 bits datos, sin paridad, 1 bit stop)
- Modo: Raw (sin procesamiento)

**Protocolo de Datos:**
```
TIPO ID VALOR
```
Donde:
- TIPO: 'T' (temperatura) o 'P' (presión)
- ID: Identificador único del sensor
- VALOR: Medición numérica

**Ejemplo:**
```
T TEMP-001 25.5
P PRES-100 101325
```

#### 7. Programa Principal (main.cpp)

**Responsabilidad:** Punto de entrada y control del flujo del programa.

**Funciones Principales:**

**capturarDatosHardware()**
- Establece conexión con Arduino
- Lee datos en ciclo infinito
- Parsea y almacena mediciones
- Crea sensores dinámicamente según datos recibidos

**desplegarMenu()**
- Muestra opciones disponibles
- Formatea salida en consola

**main()**
- Inicializa colección de sensores
- Procesa selecciones del usuario
- Gestiona ciclo de vida del sistema

**Flujo del Programa:**

```
┌─────────────────┐
│  Inicio         │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Crear Colección │
│   de Sensores   │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Mostrar Menú    │◄───────┐
└────────┬────────┘        │
         │                 │
         ▼                 │
┌─────────────────┐        │
│ Leer Opción     │        │
└────────┬────────┘        │
         │                 │
         ▼                 │
┌─────────────────┐        │
│ Procesar        │        │
│ Selección       │────────┘
└────────┬────────┘
         │
         ▼ (Opción 5)
┌─────────────────┐
│ Liberar Memoria │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│    Fin          │
└─────────────────┘
```

**Opciones del Menú:**

1. **Registrar Sensor Térmico:** Crea instancia de SensorTemperatura
2. **Registrar Sensor de Presión:** Crea instancia de SensorPresion
3. **Capturar Nueva Medición:** Agrega lectura a sensor existente
4. **Procesar Datos Almacenados:** Ejecuta cálculos estadísticos
5. **Finalizar Sistema:** Libera memoria y cierra programa
6. **Conectar Arduino:** Inicia captura desde puerto serial

### Consideraciones Técnicas

#### Gestión de Memoria

El sistema implementa prácticas sólidas de gestión de memoria:

1. **Asignación Dinámica Controlada**
   - Uso de `new` para creación de nodos y sensores
   - Liberación explícita con `delete`

2. **Regla de los Tres**
   - Constructor de copia
   - Operador de asignación
   - Destructor

3. **Prevención de Fugas**
   - Liberación en destructores
   - Limpieza antes de finalizar programa

#### Portabilidad

**Sistemas Soportados:**
- Linux (Ubuntu, Debian, Fedora, etc.)
- macOS (todas las versiones recientes)

**Limitaciones:**
- Windows requiere adaptación de SerialPort.h
- Uso de POSIX termios para comunicación serial

#### Extensibilidad

El diseño permite agregar nuevos tipos de sensores fácilmente:

1. Crear clase derivada de `SensorBase`
2. Implementar `procesarLectura()` e `imprimirInfo()`
3. Definir estructura de datos interna
4. Agregar opción en menú principal

**Ejemplo:**
```cpp
class SensorHumedad : public SensorBase {
private:
    ListaSensor<float>* registroMediciones;
public:
    void procesarLectura() override {
        // Calcular humedad promedio
    }
};
```

### Conclusiones

El Sistema de Gestión de Sensores IoT representa una solución robusta y escalable para monitoreo en tiempo real. Su arquitectura orientada a objetos, uso de polimorfismo y gestión eficiente de memoria lo convierten en una base sólida para aplicaciones IoT más complejas.

**Fortalezas:**
- Diseño modular y extensible
- Gestión segura de memoria
- Interfaz intuitiva
- Documentación completa

**Áreas de Mejora Futuras:**
- Persistencia de datos en base de datos
- Interfaz gráfica
- Soporte para Windows
- Comunicación en red (MQTT, HTTP)
- Visualización de datos en tiempo real

---

**Fin del Reporte Técnico**
