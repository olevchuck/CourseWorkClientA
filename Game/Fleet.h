#pragma once

/// @file Fleet.h
/// @brief CFleet declaration header
/// @author Dmitriy Kozhevnikov
/// @date 06-June-2011

class CPlayer;

namespace Game
{
   class CPlanet;

   /// @class CFleet
   /// @brief Fleet management class

   class CFleet
   {
   public:

      CFleet(const unsigned int id, CPlanet* from, CPlanet* to,
             CPlayer* player, const unsigned long number, const double percent);
      ~CFleet();
      /// Sets percent recieved from server to make position up-to-date
      void SetPercent(const double percent);

      /// Increase percent when timer clicks
      void IncreaseWay(const double onWay);

      /// Output for GUI
      void GetPosition (double& x, double& y) const;
      double GetAngle() const;

      /// Get functions
      unsigned long GetShipCount() const;
      unsigned short GetPlayerId() const;
      CPlayer* GetPlayer() const;
      unsigned int GetId() const;

      bool ReachedDestination() const;

   private:
      struct Point
      {
         unsigned int x;
         unsigned int y;
      };

      /// Recount real coords according to percent change
      void updatePosition();

      unsigned int m_id;
      CPlanet* m_from;
      CPlanet* m_to;
      Point m_fromPl;
      Point m_toPl;

      CPlayer* m_player;
      unsigned int m_playerId;
      unsigned long m_number;

      double m_percentPassed;
      double m_actualX;
      double m_actualY;

      double m_distance; ///< Distance between planets
      bool m_reached;
   };
} // Namespace Game
