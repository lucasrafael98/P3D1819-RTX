using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class flashlight : MonoBehaviour
{
    private bool active = false;
    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.F)){
            gameObject.GetComponent<Light>().enabled = active;
            active = !active;
        }
    }
}
