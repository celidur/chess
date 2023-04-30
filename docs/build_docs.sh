cd chess || exit
doxygen ../docs/Doxyfile
cd .. || exit
test -d docs/venv || python3 -m venv docs/venv
source ./docs/venv/bin/activate
pip install -U sphinx==6.2.1 breathe m2r2 furo
sphinx-build -b html ./docs ./docs/html -D html_theme=furo || exit
xdg-open docs/html/index.html