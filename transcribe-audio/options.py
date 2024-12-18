import tkinter as tk

def choose_model_base_on_language(chosen_language):
    """Prompt the user to choose a Whisper model using a dropdown menu."""
    models = ["tiny", "base", "small", "medium"]
    if chosen_language == "es":
        models.append("large")
    
    model_window = tk.Tk()
    model_window.title("Choose Whisper Model")
    model_window.eval('tk::PlaceWindow . center')

    model_var = tk.StringVar(value="medium")
    tk.Label(model_window, text="Select a Whisper model").pack(pady=10)
    model_menu = tk.OptionMenu(model_window, model_var, *models)
    model_menu.pack(pady=10)
    tk.Button(model_window, text="OK", command=model_window.quit).pack(pady=10)
    model_window.mainloop()

    selected_model = model_var.get()
    model_window.destroy()
    return selected_model


def choose_language():
    """Prompt the user to choose the transcription language (English or Spanish)."""
    languages = {"English": "en", "Spanish": "es"}


    lang_window = tk.Tk()
    lang_window.title("Choose Language")
    lang_window.eval('tk::PlaceWindow . center')

    lang_var = tk.StringVar(value="es")
    tk.Label(lang_window, text="Select a transcription language").pack(pady=10)
    lang_menu = tk.OptionMenu(lang_window, lang_var, *languages.keys())
    lang_menu.pack(pady=10)
    tk.Button(lang_window, text="OK", command=lang_window.quit).pack(pady=10)
    lang_window.mainloop()

    selected_language = languages[lang_var.get()]
    lang_window.destroy()
    return selected_language