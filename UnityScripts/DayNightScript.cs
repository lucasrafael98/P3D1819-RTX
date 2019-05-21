using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DayNightScript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Cursor.visible = false;
        gameObject.transform.Rotate(0,0,30f, Space.World);
    }

    // Update is called once per frame
    void Update()
    {
        gameObject.transform.Rotate(0,0,-0.02f, Space.World);
    }
}
