import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';

function init() {
    // Scene, Camera, Renderer
    const scene = new THREE.Scene();
    const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
    const renderer = new THREE.WebGLRenderer();

    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    // Rectangles (BoxGeometry)
    const rectangles: THREE.Mesh[] = [];
    const materials = [
        new THREE.MeshStandardMaterial({ color: 0xff0000 }),
        new THREE.MeshStandardMaterial({ color: 0x00ff00 }),
        new THREE.MeshStandardMaterial({ color: 0x0000ff }),
    ];

    for (let i = 0; i < 3; i++) {
        const geometry = new THREE.BoxGeometry(1, 2, 0.1); // Width, Height, Depth
        const material = materials[i % materials.length];
        const rectangle = new THREE.Mesh(geometry, material);

        rectangle.position.x = i * 2 - 2; // Spread rectangles in the X direction
        scene.add(rectangle);
        rectangles.push(rectangle);
    }

    // Light
    const light = new THREE.DirectionalLight(0xffffff, 1);
    light.position.set(5, 5, 5).normalize();
    scene.add(light);

    // Camera position
    camera.position.z = 5;

    // OrbitControls for rotation
    const controls = new OrbitControls(camera, renderer.domElement);

    // Animation Loop
    function animate() {
        requestAnimationFrame(animate);

        // Rotate rectangles
        rectangles.forEach((rectangle, index) => {
            rectangle.rotation.x += 0.01;
            rectangle.rotation.y += 0.01 * (index + 1); // Different speeds for variety
        });

        controls.update();
        renderer.render(scene, camera);
    }

    animate();

    // Handle resizing
    window.addEventListener('resize', () => {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
        renderer.setSize(window.innerWidth, window.innerHeight);
    });
}

init();