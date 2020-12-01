all: lancer
lancer:
	python main.py
doc: genere_doc deplace_doc
genere_doc :
	pydoc -w src.item_rss
	pydoc -w src.crawler
	pydoc -w src.database_tool
	pydoc -w src.elastic_tool
deplace_doc:
	move /Y src.item_rss.html .\doc

clean :
	python clean.py
	del /F /Q database\*