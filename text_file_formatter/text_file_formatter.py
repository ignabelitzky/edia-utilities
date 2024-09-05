import tkinter as tk
from tkinter import filedialog, messagebox

def insert_new_lines(input_file, output_file, words_per_line=13):
    try:
        with open(input_file, 'r') as infile:
            text = infile.read()
        
        words = text.split()
        lines = []

        for i in range(0, len(words), words_per_line):
            line = ' '.join(words[i:i+words_per_line])
            lines.append(line)
        
        with open(output_file, 'w') as outfile:
            outfile.write('\n'.join(lines))
        
        messagebox.showinfo("Success", f"File processed successfully!\nSaved as: {output_file}")
    except Exception as e:
        messagebox.showerror('Error', f'An error occurred: {e}')

def select_input_file():
    file_path = filedialog.askopenfilename(filetypes=[('Text files', '*.txt')])
    if file_path:
        input_file_entry.delete(0, tk.END)
        input_file_entry.insert(0, file_path)

def select_output_file():
    file_path = filedialog.asksaveasfilename(defaultextension='.txt', filetypes=[('Text files', '*.txt')])
    if file_path:
        output_file_entry.delete(0, tk.END)
        output_file_entry.insert(0, file_path)


def process_files():
    input_file = input_file_entry.get()
    output_file = output_file_entry.get()
    if input_file and output_file:
        insert_new_lines(input_file, output_file)
    else:
        messagebox.showwarning('Input Error', 'Please select both input and output files.')

# SEt up the main window
root = tk.Tk()
root.title("Text File Formatter")

# Create UI elements
tk.Label(root, text='Input File:').grid(row=0, column=0, padx=10, pady=5)
input_file_entry = tk.Entry(root, width=50)
input_file_entry.grid(row=0, column=1, padx=10, pady=5)
input_file_button = tk.Button(root, text='Browse', command=select_input_file)
input_file_button.grid(row=0, column=2, padx=10, pady=5)

tk.Label(root, text="Output File:").grid(row=1, column=0, padx=10, pady=5)
output_file_entry = tk.Entry(root, width=50)
output_file_entry.grid(row=1, column=1, padx=10, pady=5)
output_file_button = tk.Button(root, text="Browse", command=select_output_file)
output_file_button.grid(row=1, column=2, padx=10, pady=5)

process_button = tk.Button(root, text="Process File", command=process_files)
process_button.grid(row=2, column=0, columnspan=3, pady=10)

root.mainloop()