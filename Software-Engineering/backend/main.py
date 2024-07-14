from flask import Flask
from controller import bluep

app = Flask(__name__)
app.config["SECRET_KEY"] = "asgfddasdasdasgerher"

app.register_blueprint(bluep)
app.run(host='127.0.0.1', port=5678, debug=False)
