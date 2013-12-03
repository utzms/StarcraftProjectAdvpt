Hier sind ein paar Erläuterungen zu dem Aufbau der Daten:

Units:
	-Die ersten 5 Spalten werd ich wohl nicht erklären müssen, deswegen ignorier ich die jetzt.
	-In der Spalte BuildFrom stehen die Buildings/Units die die entsprechenden Units bauen können. Da man Buildings upgraden kann, diese aber trotzdem weiterhin die vorherigen Units bauen können, reicht es, wenn eines der genannten Dinge vorhanden ist.
	-Die Spalte Requirements stellen die zusätzlichen Requirements dar, zu den Buildings/Units, mit denen die Units produziert werden. Diese bleiben allerdings nach dem Herstellungsprozess erhalten.
	-Weiterhin gibt es die Vanish_Req Spalte. Dort werden die Units aufgelistet, die benötigt werden, bei der Erzeugung der neuen Unit allerdings verschwinden. (z.B. bei Protoss die Archon, und natürlich bei den Zerg häufig anzutreffen)


Buildings:
	-Hier sind die Dateien genauso aufgelistet, jedoch ist die Reihenfolge zwischen der Supply und BuildTime Spalte umgekehrt. Beim einlesen also aufpassen!
	-Weiterhin gibt es keine BuildFrom-Spalte.



