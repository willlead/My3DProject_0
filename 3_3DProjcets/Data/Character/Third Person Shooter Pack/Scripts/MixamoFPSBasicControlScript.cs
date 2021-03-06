using UnityEngine;
using System.Collections;

public class MixamoFPSBasicControlScript : MonoBehaviour, Mixamo.TransitionHandler {
	
	
	public bool CanTransitionTo (string guard, string source, string destination)
	{
		return true;
	}
	
	public string[] KeyControls() {
		return keys;
	}
	
	
	AnimationStateMachine GetASM() {
		return this.GetComponent<AnimationStateMachine>();
	}
	
	// Use this for initialization


	void Start () {
		GetASM().SetTransitionHandler( this );
		controller = GetComponent<CharacterController>();
		asm = GetASM();
	}
	
	public bool ShowGUIKey = true;
	private float gravity = 9.81f;
	public float turnDegrees = 72f;
	private AnimationStateMachine asm;
	private AnimationStateMachine.RootMotionResult result;
	private CharacterController controller;
	private bool upperbodyLayer = true;
	private Vector3 moveDirection = Vector3.zero;
	private string[] keys ={
		"W", "Forward",
		"S", "Backwards",
		"A", "Strafe Left",
		"D", "Strafe Right",
		"Shift", "run",
		"Q", "Turn Left",
		"E", "Turn Right",
		"Left Mouse", "Shoot",
		"R", "Reload",
		"F", "Granade",
		"H", "Hitreaction"
	};
	
	void OnGUI() {
		if( ShowGUIKey ) {
			GUILayout.BeginVertical( GUILayout.ExpandWidth( true ) );
			GUILayout.Label( "Key Options:" );
			
			// loop keys
			
			for (int i = 0; i < keys.Length; i += 2) {
				GUILayout.Label(keys[i] + " - " + keys[i+1]);
			}

			GUILayout.EndVertical();
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	//Apply gravity  
	moveDirection.y = controller.velocity.y - gravity * Time.deltaTime;
	
	AnimationStateMachine asm = GetASM();
		int turnDirection = 0;
			if( Input.GetKey( KeyCode.W ) ) {
				if( Input.GetKey( KeyCode.LeftShift ) || Input.GetKey( KeyCode.RightShift ) ) {
						asm.ControlWeights["ctrl_move"] = Mixamo.Util.CrossFadeDown( asm.ControlWeights["ctrl_move"] , 0.3f );
					} else {
						asm.ControlWeights["ctrl_move"] = Mixamo.Util.CrossFadeUp( asm.ControlWeights["ctrl_move"] , 0.3f );
					}
				asm.ChangeState( "move" );
					
			} else if( Input.GetKey( KeyCode.S ) ) {
				if( Input.GetKey( KeyCode.LeftShift ) || Input.GetKey( KeyCode.RightShift ) ) 
					asm.ChangeState( "run_backwards" );
				
				else 
					asm.ChangeState( "walk_backwards" );
			} else if( Input.GetKey( KeyCode.A ) ) {
				if( Input.GetKey( KeyCode.LeftShift ) || Input.GetKey( KeyCode.RightShift ) ) 
					asm.ChangeState( "strafe_left" );
				
				else 
					asm.ChangeState( "walk_strafe_left" );
			} 
			
			else if( Input.GetKey( KeyCode.D ) ) {
				if( Input.GetKey( KeyCode.LeftShift ) || Input.GetKey( KeyCode.RightShift ) ) 
					asm.ChangeState( "strafe_right" );
				
				else 
					asm.ChangeState( "walk_strafe_right" );
			} 
			
			else if( Input.GetKey( KeyCode.Q ) ) {
				asm.ChangeState("turn_left");
			} else if( Input.GetKey( KeyCode.E ) ) {
				asm.ChangeState("turn_right");
			}
			 
			
			else {
					asm.ChangeState( 0, "idle" );
			}
			
		
		// Upperbody Layer Controls that change based on the states above. Working along the same logic, if the player is crouched
		// The states the play are the states available to a crouching player.
			if (upperbodyLayer){
				if (Input.GetKeyDown (KeyCode.R)){
					asm.ChangeState( 1, "reload");
				}	if (Input.GetButton ("Fire1")){
					asm.ChangeState( 1, "fire");
				}		else{
						asm.ChangeState( 1, "nothing");
				}	if (Input.GetKeyDown (KeyCode.F)){
					asm.ChangeState( 1, "granade");
				}	if (Input.GetKeyDown (KeyCode.H)){
					asm.ChangeState( 1, "hitreaction");
				}
			}
		
		if( Input.GetKey( KeyCode.Q ) ) {
			turnDirection = -1;
		} else if( Input.GetKey( KeyCode.E ) ) {
			turnDirection = 1;
		}
		
		if( turnDirection != 0f ){
			Vector3 forward = this.transform.forward;
			forward.y = 0;
			forward = forward.normalized;
			Vector3 right = new Vector3(forward.z, 0, -forward.x);
			transform.rotation = Quaternion.LookRotation( Vector3.RotateTowards( forward , right * turnDirection , turnDegrees * Mathf.Deg2Rad * Time.deltaTime , 1000f ) );
		}
	}
	
	void LateUpdate() {
		if (controller != null){
			result = asm.GetRootMotion();
			if( result != null) {
				controller.Move((moveDirection * Time.deltaTime) + result.GlobalTranslation);
			}
		}
	}
}
