using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerUI : MonoBehaviour
{

    [SerializeField]
    RectTransform healthBarFill;

    private PlayerHP player;
    void Start(){
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerHP>();
    }

    // Update is called once per frame
    void Update()
    {
        SetHealthAmount(player.getHealthPer());
    }

    void SetHealthAmount(float amount){
        healthBarFill.localScale = new Vector3(1f,amount,1f);
    }
}
