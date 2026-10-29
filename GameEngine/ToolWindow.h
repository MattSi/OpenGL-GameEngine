#pragma once


#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Engine.h"
#include "RenderSystem.h"
#include "OrbitCameraComponent.h"
#include "OrbitRotationComponent.h"
#include "CameraComponent.h"
#include "InstancedModelRenderer.h"


namespace GameEngine {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ToolWindow
	/// </summary>
	public ref class ToolWindow : public System::Windows::Forms::Form
	{
	public:
		ToolWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ToolWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ btnResetLightPosition;
	protected:

	protected:
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TrackBar^ tbShipRotationRate;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TrackBar^ tbSpecularStrength;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TrackBar^ tbSpecularR;

	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ lblR;
	private: System::Windows::Forms::Label^ lblG;


	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TrackBar^ tbSpecularG;
	private: System::Windows::Forms::Label^ lblB;


	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::TrackBar^ tbSpecularB;
	private: System::Windows::Forms::RadioButton^ rbMoveLight;
	private: System::Windows::Forms::RadioButton^ rbTransform;
	private: System::Windows::Forms::Button^ btmResetTransform;




	private: System::Windows::Forms::CheckBox^ checkBox1;
	private: System::Windows::Forms::CheckBox^ checkBox2;
	private: System::Windows::Forms::CheckBox^ checkBox3;
	private: System::Windows::Forms::Label^ lblShipRotationRate;
	private: System::Windows::Forms::Label^ lblSpecularStrength;
	private: System::Windows::Forms::RadioButton^ rbWaterScene;



	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::TrackBar^ tbFrequency;

	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::TrackBar^ tbAmpitude;

	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::CheckBox^ checkBox4;
	private: System::Windows::Forms::CheckBox^ checkBox5;
	private: System::Windows::Forms::RadioButton^ rbSpaceScene;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::TrackBar^ trackBar2;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TrackBar^ trackBar3;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label16;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnResetLightPosition = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tbShipRotationRate = (gcnew System::Windows::Forms::TrackBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tbSpecularStrength = (gcnew System::Windows::Forms::TrackBar());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tbSpecularR = (gcnew System::Windows::Forms::TrackBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lblR = (gcnew System::Windows::Forms::Label());
			this->lblG = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->tbSpecularG = (gcnew System::Windows::Forms::TrackBar());
			this->lblB = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->tbSpecularB = (gcnew System::Windows::Forms::TrackBar());
			this->rbMoveLight = (gcnew System::Windows::Forms::RadioButton());
			this->rbTransform = (gcnew System::Windows::Forms::RadioButton());
			this->btmResetTransform = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->lblShipRotationRate = (gcnew System::Windows::Forms::Label());
			this->lblSpecularStrength = (gcnew System::Windows::Forms::Label());
			this->rbWaterScene = (gcnew System::Windows::Forms::RadioButton());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->tbFrequency = (gcnew System::Windows::Forms::TrackBar());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->tbAmpitude = (gcnew System::Windows::Forms::TrackBar());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->rbSpaceScene = (gcnew System::Windows::Forms::RadioButton());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->trackBar3 = (gcnew System::Windows::Forms::TrackBar());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbShipRotationRate))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularStrength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularR))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularG))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularB))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbFrequency))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbAmpitude))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->BeginInit();
			this->SuspendLayout();
			// 
			// btnResetLightPosition
			// 
			this->btnResetLightPosition->Location = System::Drawing::Point(12, 220);
			this->btnResetLightPosition->Name = L"btnResetLightPosition";
			this->btnResetLightPosition->Size = System::Drawing::Size(117, 20);
			this->btnResetLightPosition->TabIndex = 0;
			this->btnResetLightPosition->Text = L"Reset Light Position";
			this->btnResetLightPosition->UseVisualStyleBackColor = true;
			this->btnResetLightPosition->Click += gcnew System::EventHandler(this, &ToolWindow::btnResetLightPosition_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(8, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(97, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Ship Rotation Rate";
			// 
			// tbShipRotationRate
			// 
			this->tbShipRotationRate->Location = System::Drawing::Point(11, 25);
			this->tbShipRotationRate->Name = L"tbShipRotationRate";
			this->tbShipRotationRate->Size = System::Drawing::Size(314, 45);
			this->tbShipRotationRate->TabIndex = 2;
			this->tbShipRotationRate->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbShipRotationRate_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(9, 57);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(92, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Specular Strength";
			// 
			// tbSpecularStrength
			// 
			this->tbSpecularStrength->Location = System::Drawing::Point(11, 73);
			this->tbSpecularStrength->Maximum = 128;
			this->tbSpecularStrength->Minimum = 1;
			this->tbSpecularStrength->Name = L"tbSpecularStrength";
			this->tbSpecularStrength->Size = System::Drawing::Size(314, 45);
			this->tbSpecularStrength->SmallChange = 4;
			this->tbSpecularStrength->TabIndex = 4;
			this->tbSpecularStrength->TickFrequency = 10;
			this->tbSpecularStrength->Value = 1;
			this->tbSpecularStrength->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbSpecularStrength_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(9, 105);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Specular Colour";
			// 
			// tbSpecularR
			// 
			this->tbSpecularR->Location = System::Drawing::Point(20, 121);
			this->tbSpecularR->Maximum = 100;
			this->tbSpecularR->Name = L"tbSpecularR";
			this->tbSpecularR->Size = System::Drawing::Size(291, 45);
			this->tbSpecularR->SmallChange = 3;
			this->tbSpecularR->TabIndex = 6;
			this->tbSpecularR->TickFrequency = 10;
			this->tbSpecularR->Value = 100;
			this->tbSpecularR->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbSpecularR_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(8, 121);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(15, 13);
			this->label4->TabIndex = 7;
			this->label4->Text = L"R";
			// 
			// lblR
			// 
			this->lblR->AutoSize = true;
			this->lblR->Location = System::Drawing::Point(327, 121);
			this->lblR->Name = L"lblR";
			this->lblR->Size = System::Drawing::Size(28, 13);
			this->lblR->TabIndex = 8;
			this->lblR->Text = L"1.00";
			// 
			// lblG
			// 
			this->lblG->AutoSize = true;
			this->lblG->Location = System::Drawing::Point(327, 144);
			this->lblG->Name = L"lblG";
			this->lblG->Size = System::Drawing::Size(28, 13);
			this->lblG->TabIndex = 11;
			this->lblG->Text = L"1.00";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(8, 144);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(15, 13);
			this->label7->TabIndex = 10;
			this->label7->Text = L"G";
			// 
			// tbSpecularG
			// 
			this->tbSpecularG->Location = System::Drawing::Point(20, 144);
			this->tbSpecularG->Maximum = 100;
			this->tbSpecularG->Name = L"tbSpecularG";
			this->tbSpecularG->Size = System::Drawing::Size(291, 45);
			this->tbSpecularG->SmallChange = 3;
			this->tbSpecularG->TabIndex = 9;
			this->tbSpecularG->TickFrequency = 10;
			this->tbSpecularG->Value = 100;
			this->tbSpecularG->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbSpecularG_ValueChanged);
			// 
			// lblB
			// 
			this->lblB->AutoSize = true;
			this->lblB->Location = System::Drawing::Point(327, 169);
			this->lblB->Name = L"lblB";
			this->lblB->Size = System::Drawing::Size(28, 13);
			this->lblB->TabIndex = 14;
			this->lblB->Text = L"1.00";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(8, 169);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(14, 13);
			this->label9->TabIndex = 13;
			this->label9->Text = L"B";
			// 
			// tbSpecularB
			// 
			this->tbSpecularB->Location = System::Drawing::Point(20, 169);
			this->tbSpecularB->Maximum = 100;
			this->tbSpecularB->Name = L"tbSpecularB";
			this->tbSpecularB->Size = System::Drawing::Size(291, 45);
			this->tbSpecularB->SmallChange = 3;
			this->tbSpecularB->TabIndex = 12;
			this->tbSpecularB->TickFrequency = 10;
			this->tbSpecularB->Value = 100;
			this->tbSpecularB->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbSpecularB_ValueChanged);
			// 
			// rbMoveLight
			// 
			this->rbMoveLight->AutoSize = true;
			this->rbMoveLight->Location = System::Drawing::Point(11, 197);
			this->rbMoveLight->Name = L"rbMoveLight";
			this->rbMoveLight->Size = System::Drawing::Size(78, 17);
			this->rbMoveLight->TabIndex = 15;
			this->rbMoveLight->Text = L"Move Light";
			this->rbMoveLight->UseVisualStyleBackColor = true;
			this->rbMoveLight->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::rbMoveLight_CheckedChanged);
			// 
			// rbTransform
			// 
			this->rbTransform->AutoSize = true;
			this->rbTransform->Location = System::Drawing::Point(12, 246);
			this->rbTransform->Name = L"rbTransform";
			this->rbTransform->Size = System::Drawing::Size(72, 17);
			this->rbTransform->TabIndex = 17;
			this->rbTransform->TabStop = true;
			this->rbTransform->Text = L"Transform";
			this->rbTransform->UseVisualStyleBackColor = true;
			this->rbTransform->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::rbTransform_CheckedChanged);
			// 
			// btmResetTransform
			// 
			this->btmResetTransform->Location = System::Drawing::Point(11, 269);
			this->btmResetTransform->Name = L"btmResetTransform";
			this->btmResetTransform->Size = System::Drawing::Size(117, 20);
			this->btmResetTransform->TabIndex = 16;
			this->btmResetTransform->Text = L"Reset Transform";
			this->btmResetTransform->UseVisualStyleBackColor = true;
			this->btmResetTransform->Click += gcnew System::EventHandler(this, &ToolWindow::btmResetTransform_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(134, 272);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(70, 17);
			this->checkBox1->TabIndex = 18;
			this->checkBox1->Text = L"Translate";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::checkBox1_CheckedChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(210, 272);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(58, 17);
			this->checkBox2->TabIndex = 19;
			this->checkBox2->Text = L"Rotate";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::checkBox2_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(272, 272);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(53, 17);
			this->checkBox3->TabIndex = 20;
			this->checkBox3->Text = L"Scale";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::checkBox3_CheckedChanged);
			// 
			// lblShipRotationRate
			// 
			this->lblShipRotationRate->AutoSize = true;
			this->lblShipRotationRate->Location = System::Drawing::Point(331, 25);
			this->lblShipRotationRate->Name = L"lblShipRotationRate";
			this->lblShipRotationRate->Size = System::Drawing::Size(13, 13);
			this->lblShipRotationRate->TabIndex = 21;
			this->lblShipRotationRate->Text = L"0";
			// 
			// lblSpecularStrength
			// 
			this->lblSpecularStrength->AutoSize = true;
			this->lblSpecularStrength->Location = System::Drawing::Point(331, 73);
			this->lblSpecularStrength->Name = L"lblSpecularStrength";
			this->lblSpecularStrength->Size = System::Drawing::Size(13, 13);
			this->lblSpecularStrength->TabIndex = 22;
			this->lblSpecularStrength->Text = L"0";
			// 
			// rbWaterScene
			// 
			this->rbWaterScene->AutoSize = true;
			this->rbWaterScene->Location = System::Drawing::Point(11, 295);
			this->rbWaterScene->Name = L"rbWaterScene";
			this->rbWaterScene->Size = System::Drawing::Size(88, 17);
			this->rbWaterScene->TabIndex = 23;
			this->rbWaterScene->TabStop = true;
			this->rbWaterScene->Text = L"Water Scene";
			this->rbWaterScene->UseVisualStyleBackColor = true;
			this->rbWaterScene->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::rbWaterScene_CheckedChanged);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(13, 315);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(57, 13);
			this->label12->TabIndex = 24;
			this->label12->Text = L"Frequency";
			// 
			// tbFrequency
			// 
			this->tbFrequency->Location = System::Drawing::Point(11, 331);
			this->tbFrequency->Maximum = 400;
			this->tbFrequency->Name = L"tbFrequency";
			this->tbFrequency->Size = System::Drawing::Size(314, 45);
			this->tbFrequency->SmallChange = 3;
			this->tbFrequency->TabIndex = 25;
			this->tbFrequency->TickFrequency = 40;
			this->tbFrequency->Scroll += gcnew System::EventHandler(this, &ToolWindow::tbFrequency_Scroll);
			this->tbFrequency->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbFrequency_ValueChanged);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(327, 331);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(13, 13);
			this->label13->TabIndex = 26;
			this->label13->Text = L"0";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(13, 363);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(53, 13);
			this->label14->TabIndex = 27;
			this->label14->Text = L"Amplitude";
			// 
			// tbAmpitude
			// 
			this->tbAmpitude->Location = System::Drawing::Point(11, 382);
			this->tbAmpitude->Maximum = 100;
			this->tbAmpitude->Name = L"tbAmpitude";
			this->tbAmpitude->Size = System::Drawing::Size(314, 45);
			this->tbAmpitude->SmallChange = 3;
			this->tbAmpitude->TabIndex = 28;
			this->tbAmpitude->TickFrequency = 10;
			this->tbAmpitude->Scroll += gcnew System::EventHandler(this, &ToolWindow::tbAmpitude_Scroll);
			this->tbAmpitude->ValueChanged += gcnew System::EventHandler(this, &ToolWindow::tbAmpitude_ValueChanged);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(327, 382);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(13, 13);
			this->label15->TabIndex = 29;
			this->label15->Text = L"0";
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Location = System::Drawing::Point(21, 423);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(112, 17);
			this->checkBox4->TabIndex = 30;
			this->checkBox4->Text = L"Wireframe Render";
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::checkBox4_CheckedChanged);
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Location = System::Drawing::Point(156, 423);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(68, 17);
			this->checkBox5->TabIndex = 31;
			this->checkBox5->Text = L"Tint Blue";
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::checkBox5_CheckedChanged);
			// 
			// rbSpaceScene
			// 
			this->rbSpaceScene->AutoSize = true;
			this->rbSpaceScene->Location = System::Drawing::Point(11, 456);
			this->rbSpaceScene->Name = L"rbSpaceScene";
			this->rbSpaceScene->Size = System::Drawing::Size(90, 17);
			this->rbSpaceScene->TabIndex = 32;
			this->rbSpaceScene->TabStop = true;
			this->rbSpaceScene->Text = L"Space Scene";
			this->rbSpaceScene->UseVisualStyleBackColor = true;
			this->rbSpaceScene->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::rbSpaceScene_CheckedChanged);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(30, 532);
			this->trackBar1->Maximum = 360;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(314, 45);
			this->trackBar1->TabIndex = 33;
			this->trackBar1->TickFrequency = 10;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &ToolWindow::trackBar1_Scroll);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(350, 541);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(13, 13);
			this->label6->TabIndex = 35;
			this->label6->Text = L"0";
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(26, 490);
			this->trackBar2->Maximum = 360;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(314, 45);
			this->trackBar2->TabIndex = 36;
			this->trackBar2->TickFrequency = 10;
			this->trackBar2->Scroll += gcnew System::EventHandler(this, &ToolWindow::trackBar2_Scroll);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(350, 490);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(13, 13);
			this->label5->TabIndex = 37;
			this->label5->Text = L"0";
			// 
			// trackBar3
			// 
			this->trackBar3->Location = System::Drawing::Point(30, 572);
			this->trackBar3->Maximum = 180;
			this->trackBar3->Name = L"trackBar3";
			this->trackBar3->Size = System::Drawing::Size(314, 45);
			this->trackBar3->TabIndex = 38;
			this->trackBar3->TickFrequency = 10;
			this->trackBar3->Scroll += gcnew System::EventHandler(this, &ToolWindow::trackBar3_Scroll);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(350, 572);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(13, 13);
			this->label8->TabIndex = 39;
			this->label8->Text = L"0";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(6, 490);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(31, 13);
			this->label10->TabIndex = 40;
			this->label10->Text = L"Pitch";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(6, 532);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(28, 13);
			this->label11->TabIndex = 41;
			this->label11->Text = L"Yaw";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(6, 572);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(14, 13);
			this->label16->TabIndex = 42;
			this->label16->Text = L"Z";
			// 
			// ToolWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(412, 698);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->trackBar3);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->trackBar2);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->rbSpaceScene);
			this->Controls->Add(this->checkBox5);
			this->Controls->Add(this->checkBox4);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->tbAmpitude);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->tbFrequency);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->rbWaterScene);
			this->Controls->Add(this->lblSpecularStrength);
			this->Controls->Add(this->lblShipRotationRate);
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->rbTransform);
			this->Controls->Add(this->btmResetTransform);
			this->Controls->Add(this->rbMoveLight);
			this->Controls->Add(this->lblB);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->tbSpecularB);
			this->Controls->Add(this->lblG);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->tbSpecularG);
			this->Controls->Add(this->lblR);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->tbSpecularR);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->tbSpecularStrength);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->tbShipRotationRate);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnResetLightPosition);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ToolWindow";
			this->Text = L"Tool Window";
			this->TopMost = true;
			this->Load += gcnew System::EventHandler(this, &ToolWindow::ToolWindow_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbShipRotationRate))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularStrength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularR))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularG))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbSpecularB))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbFrequency))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tbAmpitude))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ToolWindow_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void tbShipRotationRate_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int v = this->tbShipRotationRate->Value;
		this->lblShipRotationRate->Text = (v).ToString("F2");
		Engine::GetInstance().toolWindowData.ShipRotationRate = static_cast<float>(v);
	}


	private: System::Void tbSpecularStrength_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		this->lblSpecularStrength->Text = this->tbSpecularStrength->Value.ToString();
		Engine::GetInstance().toolWindowData.SpecularStrength = static_cast<float>(this->tbSpecularStrength->Value);
	}


	private: System::Void tbSpecularR_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int v = this->tbSpecularR->Value;
		float fv = static_cast<float>(v) / 100.0f;
		this->lblR->Text = fv.ToString("F2");
		Engine::GetInstance().toolWindowData.SpecularColor.r = fv;
	}

	private: System::Void tbSpecularG_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int v = this->tbSpecularG->Value;
		float fv = static_cast<float>(v) / 100.0f;
		this->lblG->Text = fv.ToString("F2");
		Engine::GetInstance().toolWindowData.SpecularColor.g = fv;
	}
	private: System::Void tbSpecularB_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int v = this->tbSpecularB->Value;
		float fv = static_cast<float>(v) / 100.0f;
		this->lblB->Text = fv.ToString("F2");
		Engine::GetInstance().toolWindowData.SpecularColor.b = fv;
	}

	private: System::Void tbFrequency_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int v = this->tbFrequency->Value;
		float fv = static_cast<float>(v) / 100.0f;
		this->label13->Text = fv.ToString("F2");
		Engine::GetInstance().toolWindowData.Frequency = fv;
	}

	private: System::Void tbAmpitude_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		int v = this->tbAmpitude->Value;
		float fv = static_cast<float>(v) / 100.0f;
		this->label15->Text = fv.ToString("F2");
		Engine::GetInstance().toolWindowData.Amplitude = fv;
	}


	private: System::Void rbMoveLight_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.IsMoveLight = this->rbMoveLight->Checked;
		RenderSystem& renderSystem = RenderSystem::GetInstance();
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* aircraft = sceneManager.GetActiveScene()->FindEntityByName("AreaCraft Model");
		renderSystem.FocusCameraOnEntity(aircraft);

	}
	private: System::Void rbTransform_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.IsTransform = this->rbTransform->Checked;
		RenderSystem& renderSystem = RenderSystem::GetInstance();
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* aircraft = sceneManager.GetActiveScene()->FindEntityByName("AreaCraft Model");
		renderSystem.FocusCameraOnEntity(aircraft);
	}
	private: System::Void rbWaterScene_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.isWaterScene = this->rbWaterScene->Checked;
		RenderSystem& renderSystem = RenderSystem::GetInstance();
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* bigfish = sceneManager.GetActiveScene()->FindEntityByName("BigFish");
		renderSystem.FocusCameraOnEntity(bigfish);
	}
	
	private: System::Void btnResetLightPosition_Click(System::Object^ sender, System::EventArgs^ e) {
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* lightEntity = sceneManager.GetActiveScene()->FindEntityByName("White Point Light");
		if (lightEntity) {
			Transform* t = lightEntity->GetTransform();
			t->ResetPosition();
			t->ResetRotation();
			t->ResetScale();
		}
	}
	private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.Translate = this->checkBox1->Checked;
	}
	private: System::Void checkBox2_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.Rotate = this->checkBox2->Checked;
	}
	private: System::Void checkBox3_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.Scale = this->checkBox3->Checked;
	}
	private: System::Void btmResetTransform_Click(System::Object^ sender, System::EventArgs^ e) {
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* airCraft = sceneManager.GetActiveScene()->FindEntityByName("AreaCraft Model");
		if (airCraft) {
			Transform* t = airCraft->GetTransform();
			t->ResetPosition();
			t->ResetRotation();
			t->ResetScale();
		}
	}
	private: System::Void tbFrequency_Scroll(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.Frequency = static_cast<float>(this->tbFrequency->Value) / 100.0f;
	}

	private: System::Void tbAmpitude_Scroll(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.Amplitude = static_cast<float>(this->tbAmpitude->Value) / 100.0f;
	}
	private: System::Void checkBox4_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.WireframeRendering = this->checkBox4->Checked;
	}
	private: System::Void checkBox5_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.TintBlue = this->checkBox5->Checked;
	}

	private: System::Void rbSpaceScene_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		/*Engine::GetInstance().toolWindowData.isSpaceScene = this->rbSpaceScene->Checked;
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* selfEntity = sceneManager.GetActiveScene()->FindEntityByName("Main Camera");
		Entity* targetEntity = sceneManager.GetActiveScene()->FindEntityByName("AreaCraft Model");
		Entity* fishInstanceEntity = sceneManager.GetActiveScene()->FindEntityByName("BigFishInstanced");


		OrbitRotationComponent* comp = targetEntity->GetComponent<OrbitRotationComponent>();
		CameraComponent* cameraComp = selfEntity->GetComponent<CameraComponent>();
		InstancedModelRenderer* instancedComp = fishInstanceEntity->GetComponent<InstancedModelRenderer>();

		if(this->rbSpaceScene->Checked)
		{
			comp->SetTargetEntity(selfEntity);
			cameraComp->SetTargetEntity(targetEntity);
			instancedComp->visible = true;
		}
		else
		{
			comp->SetTargetEntity(nullptr);
			cameraComp->SetTargetEntity(nullptr);
			instancedComp->visible = false;
		}*/

	}
	private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e) {
		Engine::GetInstance().toolWindowData.rotationOffsetY = static_cast<float>(this->trackBar1->Value);
		label6->Text = this->trackBar1->Value.ToString();
	}
private: System::Void trackBar3_Scroll(System::Object^ sender, System::EventArgs^ e) {
	Engine::GetInstance().toolWindowData.rotationOffsetZ = static_cast<float>(this->trackBar3->Value);
	label8->Text = this->trackBar3->Value.ToString();
}
private: System::Void trackBar2_Scroll(System::Object^ sender, System::EventArgs^ e) {
	Engine::GetInstance().toolWindowData.rotationOffsetX = static_cast<float>(this->trackBar2->Value);
	label5->Text = this->trackBar2->Value.ToString();
}
};
}
