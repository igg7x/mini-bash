# 📂 Mini-Bash en C

Un intérprete de comandos (Shell) minimalista desarrollado en C para sistemas basados en Unix/Linux. Este proyecto demuestra el uso de llamadas al sistema para la gestión de procesos, manipulación de descriptores de archivos y la arquitectura básica de un entorno de ejecución de comandos.

## 🚀 Características Principales

- **Ejecución de Binarios:** Ejecuta cualquier comando disponible en el `$PATH` del sistema (ej. `ls`, `grep`, `mkdir`).
- **Ciclo de Vida de Procesos:** Implementación del modelo `fork-exec-wait` para la creación y sincronización de subprocesos.
- **Redirección de I/O:** Soporte para redirección de salida estándar mediante el operador `>`.
- **Built-in Commands:** Comando interno `exit` para una terminación controlada del programa.
- **Automatización de Compilación:** Incluye un `Makefile` configurado con flags de seguridad (`-Wall -Wextra`).

## 🛠️ Conceptos de Sistemas Operativos Aplicados

Este proyecto profundiza en la interacción entre el software de usuario y el Kernel de Linux:

1. **Gestión de Procesos:** Uso de `fork()` para clonar el proceso padre y `execvp()` para cargar una nueva imagen ejecutable en el proceso hijo.
2. **Sincronización:** Implementación de `wait()` para recoger el estado de salida de los hijos y evitar la acumulación de procesos "zombis".
3. **Manipulación de File Descriptors:** Uso de `open()` y `dup2()` para redirigir los flujos de datos (`stdout`) hacia archivos físicos.
4. **Parsing de Strings:** Manejo de punteros y buffers en C para tokenizar la entrada del usuario.
5.**Pipes Multiproceso:** Comunicación bidireccional entre comandos (ej. `ls | grep .c`) mediante el uso de `pipe()` y sincronización de descriptores.

## 📦 Instalación y Uso
### Requisitos
- GCC (GNU Compiler Collection)
- Entorno Linux o WSL (Windows Subsystem for Linux)

### Compilación
Clona el repositorio y compila usando el `Makefile` incluido:

```bash
git clone [https://github.com/TU_USUARIO/mini-bash.git](https://github.com/TU_USUARIO/mini-bash.git)
cd mini-bash
make