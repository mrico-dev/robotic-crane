
import * as BACKEND from './backend_interface';
import * as VISUALIZER from './visualizer';

VISUALIZER.build_scene();
BACKEND.initialize_connection();
BACKEND.setup_button();
VISUALIZER.animate();
