using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;
using System;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public RectTransform[] Scores;
    float[] leaderboard = new float[4];
    bool gameEnded = false;

    [SerializeField]
    private GameObject GameOverUI;
    public GameObject HUD;

    [SerializeField]
    private GameObject DieUI;

    public Boolean treasure;

    void LateUpdate(){
        //GameObject.FindGameObjectWithTag("score").GetComponent<Text>().text = (1 / Time.timeSinceLevelLoad).ToString();
    }

    public void EndGame(){
        if(!gameEnded){
            gameEnded = true;
            GameOver();
        }
    }

    public void Die(){
        if(!gameEnded){
            gameEnded = true;
            Dead();
        }
    }

    void GameOver(){
        Cursor.visible = true;
        Cursor.lockState = CursorLockMode.None;
        float score = 1 / Time.timeSinceLevelLoad;
        if(treasure) score *= 2;
        treasure = false;
        int index = Array.IndexOf(leaderboard, leaderboard.Min());
        leaderboard[index] = Time.timeSinceLevelLoad;
        Scores[index].GetComponent<Text>().text = leaderboard[index].ToString("F3"); 
        GameOverUI.SetActive(true);
        Time.timeScale = 0.00001f;
    }

    void Dead(){
        Cursor.visible = true;
        Cursor.lockState = CursorLockMode.None;
        float score = 1 / Time.timeSinceLevelLoad;
        if(treasure) score *= 2;
        treasure = false;
        int index = Array.IndexOf(leaderboard, leaderboard.Min());
        leaderboard[index] = Time.timeSinceLevelLoad;
        Scores[index].GetComponent<Text>().text = leaderboard[index].ToString("F3"); 
        DieUI.SetActive(true);
        Time.timeScale = 0.00001f;
    }
}
