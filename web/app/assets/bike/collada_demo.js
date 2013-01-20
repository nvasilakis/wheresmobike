if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var container, stats;

var w = 300;
var h = 300;

var camera, scene, renderer, objects;
var particleLight, pointLight;
var dae, skin;

var rxz = 10;
var ry  = 2;
var rsq = rxz*rxz + ry*ry;
var rdelta = 0.1;

var loader = new THREE.ColladaLoader();
loader.options.convertUpAxis = true;
loader.load( '/assets/bicycle.dae', function ( collada ) {

    dae = collada.scene;
    skin = collada.skins[ 0 ];

    dae.scale.x = dae.scale.y = dae.scale.z = 0.15;
    dae.updateMatrix();

    init();
    animate();
} );

function init() {
    container = $('#loading-search .image');
    camera = new THREE.PerspectiveCamera( 45, w /
                                          h, 1, 2000 );
    camera.position.set( 2, 2, 3 );

    scene = new THREE.Scene();

    // Grid

    var size = 14, step = 1;


    // Add the COLLADA

    scene.add( dae );

    //				particleLight = new THREE.Mesh( new THREE.SphereGeometry( 4, 8, 8 ), new THREE.MeshBasicMaterial( { color: 0xffffff } ) );
    //				scene.add( particleLight );

    // Lights

    scene.add( new THREE.AmbientLight( 0xcccccc ) );

    //				var directionalLight = new THREE.DirectionalLight(/*Math.random() * 0xffffff*/0xeeeeee );
    //				directionalLight.position.x = Math.random() - 0.5;
    //				directionalLight.position.y = Math.random() - 0.5;
    //				directionalLight.position.z = Math.random() - 0.5;
//				directionalLight.position.normalize();
    //				scene.add( directionalLight );

//				pointLight = new THREE.PointLight( 0xffffff, 4 );
    //				pointLight.position = particleLight.position;
    //				scene.add( pointLight );

    renderer = new THREE.WebGLRenderer();
    renderer.setSize(w, h);

    container.append($(renderer.domElement));

    stats = new Stats();
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '0px';
    //				container.appendChild( stats.domElement );

    //

    window.addEventListener( 'resize', onWindowResize, false );
    document.addEventListener( 'keydown', onKeyDown, false );
}

function onWindowResize() {

    camera.aspect = w / h;
    camera.updateProjectionMatrix();

    renderer.setSize( w , h);
}

//

var t = 0;
var clock = new THREE.Clock();

function animate() {

    var delta = clock.getDelta();

    requestAnimationFrame( animate );

    if ( t > 1 ) t = 0;

    if ( skin ) {

        // guess this can be done smarter...

        // (Indeed, there are way more frames than needed and interpolation is not used at all
        //  could be something like - one morph per each skinning pose keyframe, or even less,
        //  animation could be resampled, morphing interpolation handles sparse keyframes quite well.
        //  Simple animation cycles like this look ok with 10-15 frames instead of 100 ;)

        for ( var i = 0; i < skin.morphTargetInfluences.length; i++ ) {

            skin.morphTargetInfluences[ i ] = 0;

        }

        skin.morphTargetInfluences[ Math.floor( t * 30 ) ] = 1;

        t += delta;

    }

    render();
    stats.update();

}

function onKeyDown ( event ) {

    switch( event.keyCode ) {

    case 38: /* Up Arrow */

        ry += rdelta;
        if (ry > 5) {
            ry = 5;
        }
        rxz = Math.sqrt(rsq - ry*ry);
        break;

    case 40: /* Down Arrow */

        if (ry < -5) {
            ry = -5;
        }
        ry -= rdelta;
        rxz = Math.sqrt(rsq - ry*ry);
        break;

    }

};

function render() {

    var timer = Date.now() * 0.0010;

    camera.position.x = Math.cos( timer ) * rxz;
    camera.position.y = ry;
    camera.position.z = Math.sin( timer ) * rxz;

    camera.lookAt( scene.position );

    //				particleLight.position.x = Math.sin( timer * 4 ) * 3009;
    //				particleLight.position.y = Math.cos( timer * 5 ) * 4000;
    //				particleLight.position.z = Math.cos( timer * 4 ) * 3009;

    renderer.render( scene, camera );

}
