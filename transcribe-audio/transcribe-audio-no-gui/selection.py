def choose_model():
    print("Available Whisper models:")
    print("1. tiny")
    print("2. small")
    print("3. medium")
    print("4. large")
    model_number = input("Select the number of the model you want to use: ")
    if model_number == "1":
        model_name = "tiny"
    elif model_number == "2":
        model_name = "small"
    elif model_number == "3":
        model_name = "medium"
    elif model_number == "4":
        model_name = "large"
    else:
        model_name = None
    return model_name

def choose_language():
    print("Available languages:")
    print("1. English")
    print("2. Spanish")
    print("3. French")
    language_number = input("Select the number of the language you want to transcribe: ")
    if language_number == "1":
        language = "en"
    elif language_number == "2":
        language = "es"
    elif language_number == "3":
        language = "fr"
    else:
        language = None
    return language