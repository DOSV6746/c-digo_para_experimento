import os
import random

def generate_random_matrix(rows, cols, min_val, max_val):
    return [[random.randint(min_val, max_val) for _ in range(cols)] for _ in range(rows)]

def generate_random_row(cols, min_val, max_val):
    return [random.randint(min_val, max_val) for _ in range(cols)]

def write_to_file(filepath, data):
    with open(filepath, 'w') as file:
        if isinstance(data[0], list):  # Es una matriz
            for row in data:
                file.write(" ".join(map(str, row)) + "\n")
        else:  # Es una sola fila
            file.write(" ".join(map(str, data)) + "\n")

def main():
    base_path = "./casos"  # Ruta base para las carpetas
    os.makedirs(base_path, exist_ok=True)
    
    for folder_num in range(1, 11):
        folder_name = os.path.join(base_path, f"caso_{folder_num}")
        os.makedirs(folder_name, exist_ok=True)
        
        # Matriz 26x26 para cost_insert.txt y cost_transpose.txt
        matrix = generate_random_matrix(26, 26, 1, 100)
        write_to_file(os.path.join(folder_name, "cost_insert.txt"), matrix)
        write_to_file(os.path.join(folder_name, "cost_transpose.txt"), matrix)
        
        # Una fila de 26 números aleatorios para cost_delete.txt y cost_replace.txt
        row = generate_random_row(26, 1, 100)
        write_to_file(os.path.join(folder_name, "cost_delete.txt"), row)
        write_to_file(os.path.join(folder_name, "cost_replace.txt"), row)
    
    print("Carpetas y archivos generados correctamente.")

if __name__ == "__main__":
    main()

