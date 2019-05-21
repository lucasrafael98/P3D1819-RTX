using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Enemy))]
public class HealthUI : MonoBehaviour
{

    public GameObject uiPrefab;
    public Transform target;

    Transform ui;

    float visibleTime = 5f;

    float lastMadeVisibleTime;
    Image healthSlider;
    // Start is called before the first frame update
    void Start()
    {
        foreach(Canvas c in FindObjectsOfType<Canvas>()){
            if(c.renderMode == RenderMode.WorldSpace){
                ui = Instantiate(uiPrefab,c.transform).transform;
                healthSlider = ui.GetChild(0).GetComponent<Image>();
                ui.gameObject.SetActive(false);
                break;
            }
        }

        GetComponent<Enemy>().OnHealthChanged += OnHealthChanged;
    }

    void OnHealthChanged(int maxHealth, int currentHealth){
        if (ui != null){
            ui.gameObject.SetActive(true);
            lastMadeVisibleTime = Time.time;
            float healthPer = (float) currentHealth/maxHealth;
            Debug.Log(healthPer);
            healthSlider.fillAmount = healthPer;            
            if(currentHealth <= 0){
                Destroy(ui.gameObject);
            }
        }
    }
    // Update is called once per frame
    void LateUpdate()
    {
        if (ui != null){
            ui.position = target.position;
            ui.forward = target.forward;
        }

        if(Time.time - lastMadeVisibleTime > visibleTime){
            ui.gameObject.SetActive(false);
        }
    }
}
