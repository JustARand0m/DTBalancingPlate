# Basic Git Mergerequest Workflow
## 1. neuen Branch anlegen
(Wenn man schon einen Branch hat zu 2.)
```
git checkout -b <Branchname>
```
oder
```
git branch <Branchname>
git checkout <Branchname>
```
An die Stelle von \<Branchname\> muss der Name des Branches eingefuegt werden
(erstellt einen neuen Branch und wechselt zu diesem)<br/>

## 2. Aenderungen commiten
Nachdem die Aenderungen am Code vorgenommen wurden, kann man nun diese auf dem 
neu erstellten Branch commiten.
```
git add .
git commit -m "<Commit Message>"
```
oder 
```
git commit -am "<Commit Message>"
```
Mit Git add muss man die Files angeben die gestaged werden "." staged alles.
Bei \<Commit Message\> muss eine Sinnvolle Kommitmessage eingetragen werden.

## 3. Branch pushen / Aenderungen pushen
Nun muss man den Branch pushen, oder wenn man schon einen Branch hat dann 
nur die Aenderungen pushen. <br/>
Noch kein Branch vorhanden: 
```
git push --set-upstream origin <Branchname>
```
oder
```
git push -u origin <Branchname>
```
Falls der Branch auf dem man arbeitet schon existiert: 
```
git push
```

## 4. Merge Request stellen
Sobald man gepusht hat muss man auf der Github seite im Reiter links auf 
Merge Request klicken. Nun wird einem eine Meldung angezeigt, dass man 
einen branch gepusht hat und man kann direkt ueber diesen Button 
einen Merge Request stellen. Ansonsten Button fuer neuen Merge Requst klicken.
nun die richtigen Branches auswaehlen von z.B. NeuerBranch auf master, dann 
Beschreibung hinzufuegen und jemanden auswaehlen der ueber den Merge Request 
drueber schauen soll.
