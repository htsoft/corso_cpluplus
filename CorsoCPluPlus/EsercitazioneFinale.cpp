/*
* Una fabbrica produce una serie di oggetti differenti, ognuno dei quali si trova su una
* catena di montaggio mobile distinta.
* Il numero di catene di montaggio è fissato a 5.
* Le catene possono essere attivate o disattivate.
* Ogni catena produce uno specifico oggetto, che può essere di tipo A, B, C, D o E.
* Sviluppare un'applicazione che recepisce gli oggetti in input per ogni catena di montaggio e
* e verifica se l'oggetto atteso e quello ricevuto sono corretti, memorizzando il risultato.
* L'applicazione deve visualizzare su richiesta dell'utente i seguenti dati:
* Numero di oggetti ricevuti per ogni catena di montaggio
* Numero di oggetti corretti per ogni catena di montaggio
* Numero di oggetti errati per ogni catena di montaggio separati per tipologia
* Lista degli oggetti errati con a fianco la catena su cui si è verificata l'anomalia 
* I dati sono recuperabili attraverso il file csv allegato al repository.
* Ogni colonna contiene i dati della relativa catena di montaggio.
* Tra un dato ed il successivo intercorre un tempo di 1 decimo di secondo.
* Al termine dell'elaborazione, anche se l'utente non ha effettuato alcuna richiesta
* fornire i risultati di tutte el lavorazioni come indicato sopra.
* 
* Esempio contenuto file CSV
* A;B;C;D;E     --> Riga corretta per le 5 linee
* A;E;C;C;C		--> Solo la prima e la terza linea sono corrette, la seconda, la quarta e la quinta sono errata
*/