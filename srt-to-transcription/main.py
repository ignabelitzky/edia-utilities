import re
import tkinter as tk
from tkinter import filedialog

def convert_srt_to_transcription(input_file, output_file):
    """
    Convierte un archivo .srt a un archivo de transcripción.

    :param input_file: Ruta del archivo .srt de entrada.
    :param output_file: Ruta del archivo .txt de salida.
    """
    with open(input_file, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    transcription = []
    current_text = ""
    start_time = None
    end_time = None

    for line in lines:
        line = line.strip()

        # Detecta las líneas de tiempo en format SRT
        match = re.match(r"(\d{2}:\d{2}:\d{2},\d{3}) --> (\d{2}:\d{2}:\d{2},\d{3})", line)
        if match:
            if current_text and start_time and end_time:
                # Añade el texto procesado al resultado
                transcription.append(f"[{start_time} - {end_time}] {current_text.strip()}\n")
            # Resetea las variables para un nuevo bloque
            start_time = match.group(1)
            end_time = match.group(2)
            current_text = ""
        elif line.isdigit():
            # Ignora los números de línea
            continue
        else:
            # Acumula el texto, reemplazando saltos de línea por espacios
            if current_text:
                current_text += f" {line}"
            else:
                current_text = line

    # Agrega el último bloque si existe
    if current_text:
        transcription.append(f"[{start_time} - {end_time}] {current_text.strip()}\n")

    # Escribe el archivo de transcripción
    with open(output_file, 'w', encoding='utf-8') as file:
        file.writelines(transcription)

def select_input_file():
    """Abre un cuadro de diálogo para seleccionar un archivo .srt de entrada."""
    return filedialog.askopenfilename(title="Seleccionar archivo .srt", filetypes=[("Archivos SRT", "*.srt")])

def select_output_file():
    """Abre un cuadro de diálogo para seleccionar un archivo .txt de salida."""
    return filedialog.asksaveasfilename(title="Guardar archivo de transcripción", defaultextension=".txt", filetypes=[("Archivos de texto", "*.txt")])

def main():
    """Función principal para gestionar la conversión de archivos."""
    root = tk.Tk()
    root.withdraw()

    input_file = select_input_file()
    if not input_file:
        print("No se seleccionó un archivo de entrada.")
        return

    output_file = select_output_file()
    if not output_file:
        print("No se seleccionó un archivo de salida.")
        return

    try:
        convert_srt_to_transcription(input_file, output_file)
        print(f"Archivo de transcripción guardado en: {output_file}")
    except Exception as e:
        print(f"Ocurrió un error al procesar el archivo: {e}")

if __name__ == "__main__":
    main()
