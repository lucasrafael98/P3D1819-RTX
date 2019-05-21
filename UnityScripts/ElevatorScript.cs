using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElevatorScript : MonoBehaviour
{
    private bool colliding = false;
    private BoxCollider boundsCollider;

    void Start(){
        boundsCollider = gameObject.GetComponentsInParent<BoxCollider>()[2];
    }
    void Update()
    {
        float y = gameObject.GetComponents<Collider>()[0].bounds.min.y;
        if(!colliding && y > boundsCollider.bounds.min.y){
            gameObject.transform.position = new Vector3(gameObject.transform.position.x, 
                                            gameObject.transform.position.y - 0.1f, gameObject.transform.position.z);
        }
    }
    void OnTriggerEnter(Collider other){
        if(other.tag.Equals("Player"))
            colliding = true;
    }
    void OnTriggerExit(Collider other){
        if(other.tag.Equals("Player"))
            colliding = false;
    }
    void OnTriggerStay(Collider other){
        if(other.tag.Equals("Player")){
            float y = gameObject.transform.position.y;
            y = Mathf.Clamp(y + 0.1f, boundsCollider.bounds.min.y, boundsCollider.bounds.max.y);
            gameObject.transform.position = new Vector3(gameObject.transform.position.x, 
                                            y, gameObject.transform.position.z);
        }
    }
}
