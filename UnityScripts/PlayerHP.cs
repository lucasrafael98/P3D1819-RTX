using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Rendering.PostProcessing;

public class PlayerHP : MonoBehaviour
{

    public int StartingHitPoints = 100;
    private int currentHP;
    private bool changed = false;
    private Bloom bloom = null;
    // Start is called before the first frame update
    void Start()
    {
        currentHP = StartingHitPoints;
        GameObject.FindGameObjectWithTag("SecondCamera").GetComponent<Camera>()
                        .GetComponentsInChildren<PostProcessVolume>()[0]
                        .profile.TryGetSettings<Bloom>(out bloom);
    }
    // Update is called once per frame
    void Update()
    {
        if(changed){
            changed = false;
        }
        if(bloom.intensity.value > 1.5)
            bloom.intensity.value -= 0.02f;
        if(bloom.threshold.value < 2)
            bloom.threshold.value += 0.02f;
        if(currentHP <= 0){
            FindObjectOfType<GameManager>().Die();
        }
    }

    public float getHealthPer(){
        return (float)currentHP/StartingHitPoints;
    }
    public void updateHP(int mod){
        FindObjectOfType<AudioManager>().Play("hurt");
        currentHP += mod;
        changed = true;
        if(mod < 0){
            if(bloom.intensity.value < 10)
                bloom.intensity.value -= mod / 10;
            if(bloom.threshold.value > 0.1)
                bloom.threshold.value += mod / 5;
        }
    }
}
