# LAN Party Simulator

## Autor : Andreescu Andrei-Valerian

## Descriere

**Programul simuleaza desfasurarea evenimentului LanParty organizat anual de Facultatea de Automatica si Calculatoare prin LSAC, la care participa toti studentii care vor sa isi testeze abilitatile la un anumit joc.**

- Se foloseste conceptul de lista in lista(simplu inlantuita) pentru a stoca informatii despre fiecare jucator al fiecarei echipe.
- Lista principala contine echipele.
  - Fiecare echipa are :
    - un nume;
    - un scor calculat pe baza mediei aritmetice al punctelor jucatorilor;
    - un numar de jucatori;
    - o lista secundara care contine jucatorii, fiecare jucator avand:
      - un nume si prenume;
      - un punctaj.
- Dupa stocarea datelor, se vor elimina n echipe din lista, pentru ca numarul echipelor ramase sa fie o putere al numarului 2.
- Numarul de echipe fiind puterea n a lui 2, se pot juca n runde.
  - Pentru fiecare runda se vor folosi structurile de data coada si stiva.
  - O runda e formata din meciuri in care o echipa concureaza cu alta pe baza scorului.
    - La final de meci, scorul echipei invingatoare creste, iar echipa este adaugata in stiva de castigatori.
    - Echipa pierzatoare e adaugata in stiva de pierzatori.
  - La final de runda, dupa de sunt jucate toate meciurile, castigatorii vor fi introdusi din stiva in coada de meciuri, iar pierzatorii for fi stersi din lista de echipe.
  - In final va ramane doar un castigator, dar se vor salva primele 8 echipe intr-o lista separata pentru a intocmi un clasamen.
- Pe baza primelor 8 echipe se va creea un arbore binar de cautare pentru o parcurgere mai eficienta. Se vor insera echipele in ordine descrescatoare, iar apoi for fi afisate.
- Se va creea un arbore binar de cautare echilibrat (AVL) pe aceleasi criteriu si va afisat nivelul 2 al acestui arbore.

## Comentarii asupra proiectului

- Crezi ca ai fi putut realiza o implementare mai buna?
  - Mereu exista loc de imbunatatiri.
- Ce ai invatat din realizarea acestui proiect?
  - Am invatat cum se foloseste lista in lista si am aprofundat folosirea structurilor de date:
    - coada;
    - stiva;
    - BST;
    - AVL.
  - Am invatat sa folosesc macrouri precum DIE, pentru a inchide programul in cazul unei erori la alocarea memoriei.
  - Am descoperit multe moduri in care poti lua segmentation fault sau erori pe valgrind.

## Corner cases

- Am trat cazul in care prenumele jucatorilor nu este despartit prin "-", caz care nu este acoperit de testele checker-ului.

## Utilizare program

![](https://github.com/DrescoAV/LanParty_Simulator/blob/main/LanParty%20Simulator%20Gif.gif)

### Compilarea programului

```bash
    make
```

### Rularea programului

```bash
    ./lanParty c.in d.in r.out
```

``c.in - fisier ce contine cerintele``

``d.in - fisier ce contine datele de intrare``

``r.out - fisier ce contine datele de iesire``

### Rulare checker

```bash
    ./checker.sh
```

