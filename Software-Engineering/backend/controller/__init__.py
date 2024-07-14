from flask import Blueprint

bluep = Blueprint('models', __name__)

from .Admin import *
from .Shopper import *
from .User import *
from .Visitor import *
from .Image import *