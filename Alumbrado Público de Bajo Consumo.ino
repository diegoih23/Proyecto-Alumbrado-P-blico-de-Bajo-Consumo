
/*
=========================================================
 PROYECTO:
 Sistema Inteligente de Iluminación Automática
 =========================================================
=========================================================
  DEFINICIÓN DE PINES
=========================================================*/

// Pin digital conectado al sensor PIR
#define PIR_PIN 2

// Pin analógico conectado al sensor LDR
#define LDR_PIN A0

// Pines PWM conectados a los LEDs
#define LED1 9
#define LED2 10
#define LED3 11


/*=========================================================
  VARIABLES DE CONFIGURACIÓN
===========================================================*/

// Intensidad baja de iluminación
// NOTA:
// 102/255 ≈ 40% de brillo real
int brilloBajo = 102;

// Intensidad máxima de iluminación
int brilloAlto = 255;

// Umbral para determinar oscuridad
// Valores menores indican noche
int umbralOscuridad = 500;


/*=========================================================
  VARIABLES DE CONTROL
=========================================================*/

// Guarda el tiempo en que se detectó movimiento
unsigned long tiempoInicioMovimiento = 0;

// Tiempo que las luces permanecen al máximo brillo
unsigned long duracionEncendido = 5000;

// Indica si el sistema está en modo brillo alto
bool modoAlto = false;

// Variable auxiliar para detectar cambios de estado
// del sensor PIR
int movimientoAnterior = HIGH;


/*=========================================================
  FUNCIÓN SETUP
  Se ejecuta una sola vez al iniciar el sistema
=========================================================*/
void setup() {

  // Inicializa comunicación serial
  // para monitoreo y depuración
  Serial.begin(9600);

  // Configuración del sensor PIR como entrada
  pinMode(PIR_PIN, INPUT);

  // Configuración de LEDs como salidas
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}


/*=========================================================
  FUNCIÓN LOOP
  Se ejecuta continuamente
=========================================================*/
void loop() {

  /*-------------------------------------------------------
    LECTURA DE SENSORES
  -------------------------------------------------------*/

  // Leer nivel de luz ambiental
  int luz = analogRead(LDR_PIN);

  // Leer estado del sensor PIR
  int movimiento = digitalRead(PIR_PIN);


  /*-------------------------------------------------------
    MONITOREO SERIAL
  -------------------------------------------------------*/

  Serial.print("Luz: ");
  Serial.print(luz);

  Serial.print(" | Movimiento: ");
  Serial.println(movimiento);


  /*-------------------------------------------------------
    DETERMINAR SI ES DE NOCHE
  -------------------------------------------------------*/

  // Si la luz es menor al umbral:
  // se considera oscuridad
  bool esNoche = luz < umbralOscuridad;


  /*-------------------------------------------------------
    COMPORTAMIENTO PRINCIPAL
  -------------------------------------------------------*/

  if (esNoche) {

    /*---------------------------------------------------
      DETECCIÓN DE MOVIMIENTO
    ---------------------------------------------------*/

    // Si el PIR detecta movimiento
    if (movimiento == HIGH) {

      // Activar modo brillo alto
      modoAlto = true;

      // Guardar tiempo actual
      tiempoInicioMovimiento = millis();
    }

    // Guardar estado anterior
    movimientoAnterior = movimiento;


    /*---------------------------------------------------
      CONTROL DE BRILLO
    ---------------------------------------------------*/

    // Si existe movimiento reciente
    if (modoAlto) {

      // Encender LEDs al máximo brillo
      ajustarBrillo(brilloAlto);

      // Verificar si terminó el tiempo
      if (millis() - tiempoInicioMovimiento > duracionEncendido) {

        // Regresar al modo ahorro energético
        modoAlto = false;
      }

    } else {

      // Mantener brillo bajo durante la noche
      ajustarBrillo(brilloBajo);
    }

  } else {

    /*---------------------------------------------------
      SI ES DE DÍA
    ---------------------------------------------------*/

    // Apagar completamente los LEDs
    apagarLEDs();
  }
}


/*=========================================================
  FUNCIÓN: ajustarBrillo()
  
  DESCRIPCIÓN:
  Ajusta simultáneamente el brillo de los 3 LEDs
  usando PWM.

  PARÁMETRO:
  nivel → Intensidad de brillo (0 - 255)
=========================================================*/
void ajustarBrillo(int nivel) {

  analogWrite(LED1, nivel);
  analogWrite(LED2, nivel);
  analogWrite(LED3, nivel);
}


/*=========================================================
  FUNCIÓN: apagarLEDs()

  DESCRIPCIÓN:
  Apaga completamente todos los LEDs.
=========================================================*/
void apagarLEDs() {

  ajustarBrillo(0);
}
