Pentru parsarea datelor, am folosit Jsmn(link:https://github.com/zserge/jsmn).
Jsmn parseaza datele și le reține în tokeni. Un token se obține din delimitarea prin "". Obțin tokenii după indexul lor.
La fiecare task parsez răspunsul primit la taskul precedent și obțin câmpurile de care am nevoie programatic. 
Pentru compute_get_request concatenez datele ce trebuiesc citite mai apoi din url_params punând "," între ele.
Pentru compute_get_request concatenez datele ce trebuiesc citite mai apoi din form_data punând "," între ele.
Pentru formatarea mesajului și trimiterea requestului folosesc funcțiile implementate în laborator.
