import os
import random
import string
import subprocess
import time
import psutil
import statistics

def generate_random_string(length):
    """Genera una cadena aleatoria de longitud especificada."""
    return ''.join(random.choices(string.ascii_lowercase, k=length))

def monitor_memory_and_execute(command, input_data):
    """Monitorea el uso de memoria y mide el tiempo de ejecución de un comando."""
    try:
        # Inicia el proceso
        process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        start_time = time.time()

        # Escribe la entrada y cierra stdin
        stdout, stderr = process.communicate(input=input_data.encode())
        
        # Medir memoria mientras el proceso corre
        max_memory = 0
        max_vms = 0  # Memoria virtual máxima
        while process.poll() is None:
            proc = psutil.Process(process.pid)
            memory_info = proc.memory_info()
            max_memory = max(max_memory, memory_info.rss)  # RSS: memoria residente
            max_vms = max(max_vms, memory_info.vms)  # VMS: memoria virtual
            time.sleep(0.01)

        end_time = time.time()

        return stdout.decode(), stderr.decode(), end_time - start_time, max_memory, max_vms
    except Exception as e:
        print(f"Error durante la ejecución: {e}")
        return "", f"Error: {e}", 0, 0, 0

def main():
    # Rutas
    cpp_program = "./algoco2"  # Asegúrate de compilar el programa C++ con este nombre
    results_file = "execution_results.txt"

    # Verifica que el programa en C++ exista
    if not os.path.exists(cpp_program):
        print(f"Error: No se encontró el programa C++ en {cpp_program}")
        return

    # Generar 10 pares de cadenas aleatorias
    input_pairs = [
        (generate_random_string(random.randint(1, 13)), generate_random_string(random.randint(1, 13))) 
        for _ in range(10)
    ]

    with open(results_file, "w") as result_file:
        result_file.write("Word1,Word2,MeanExecutionTime(s),MeanMemoryUsed(bytes),MeanVMSize(bytes)\n")
        
        for word1, word2 in input_pairs:
            input_data = f"{word1}\n{word2}\n"
            print(f"Running for: {word1}, {word2}")
            
            execution_times = []
            memory_usages = []
            vms_usages = []

            for i in range(10):  # Ejecutar 10 veces
                stdout, stderr, exec_time, memory_used, vms_used = monitor_memory_and_execute([cpp_program], input_data)
                
                if stderr:
                    print(f"Error encountered: {stderr.strip()}")
                    execution_times.append(float('inf'))  # Asignar un tiempo alto para errores
                    memory_usages.append(0)
                    vms_usages.append(0)
                else:
                    execution_times.append(exec_time)
                    memory_usages.append(memory_used)
                    vms_usages.append(vms_used)

            # Calcular medias (descartando errores si los hubo)
            valid_times = [t for t in execution_times if t != float('inf')]
            mean_exec_time = statistics.mean(valid_times) if valid_times else float('inf')
            mean_memory_used = statistics.mean(memory_usages) if memory_usages else 0
            mean_vms_used = statistics.mean(vms_usages) if vms_usages else 0

            result_file.write(f"{word1},{word2},{mean_exec_time:.6f},{mean_memory_used},{mean_vms_used}\n")

    print(f"Results saved in {results_file}")

if __name__ == "__main__":
    main()

